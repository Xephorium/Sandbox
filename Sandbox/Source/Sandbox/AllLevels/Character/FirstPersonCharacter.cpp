// Copyright Epic Games, Inc. All Rights Reserved.

#include "FirstPersonCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "AllLevels/Input/InputUtility.h"
#include "AllLevels/Utility/LogUtility.h"
#include "AllLevels/Input/GamepadLookAdapter.h"
#include "Dependencies/Steam/SteamInputComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

/*
 *  FirstPersonCharacter.cpp                           Chris Cruzen
 *  Sandbox                                              05.29.2022
 *
 *    FirstPersonCharacter is the base character class of the first
 *  person levels of Sandbox. It handles common logic for input and
 *  player movement.
 */


/*--- Lifecycle Functions ---*/

AFirstPersonCharacter::AFirstPersonCharacter() {

	// Setup Character
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 0.0f, 58.5f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Initialize Movement Variables
	IsFlying = false;

	// Create SteamInputComponent
	SteamInputComponent = CreateDefaultSubobject<USteamInputComponent>(TEXT("SteamInputComponent"));

	// Create SteamInputComponent
	GamepadLookAdapter = CreateDefaultSubobject<UGamepadLookAdapter>(TEXT("GamepadLookAdapter"));
	
}

void AFirstPersonCharacter::BeginPlay() {
	Super::BeginPlay();

	SetupSteamInputComponent();
}

void AFirstPersonCharacter::Tick(float DeltaSeconds) {
	if (SteamInputComponent->IsSteamInputAvailable()) {
		SteamInputComponent->OnTick(DeltaSeconds);
	} else {
		OnStickLook(CurrentLookInput);
	}
}


/*--- Input Setup Functions ---*/

void AFirstPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFirstPersonCharacter::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFirstPersonCharacter::OnMoveRight);
	PlayerInputComponent->BindAxis("StickLookRight", this, &AFirstPersonCharacter::OnStickLookX);
	PlayerInputComponent->BindAxis("StickLookUp", this, &AFirstPersonCharacter::OnStickLookY);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFirstPersonCharacter::OnJumpPress);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFirstPersonCharacter::OnJumpRelease);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AFirstPersonCharacter::SetupSteamInputComponent() {
	SteamInputComponent->SetupSteamInput();

	SteamInputComponent->BindMove(this, FName("OnStickMove"));
	SteamInputComponent->BindLook(this, FName("OnStickLook"));
	SteamInputComponent->BindCrouchPress(this, FName("OnCrouchPress"));
	SteamInputComponent->BindCrouchRelease(this, FName("OnCrouchRelease"));
	SteamInputComponent->BindJumpPress(this, FName("OnJumpPress"));
	SteamInputComponent->BindJumpRelease(this, FName("OnJumpRelease"));
	SteamInputComponent->BindRunPress(this, FName("OnRunPress"));
	SteamInputComponent->BindRunRelease(this, FName("OnRunRelease"));
	SteamInputComponent->BindFlyPress(this, FName("OnFlyPress"));
	SteamInputComponent->BindFlyRelease(this, FName("OnFlyRelease"));
}


/*--- Input Handlong Functions ---*/

void AFirstPersonCharacter::OnMoveForward(float Value) {
	if (Value != 0.0f) {
		if (IsFlying) AddMovementInput(FirstPersonCameraComponent->GetForwardVector(), Value);
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFirstPersonCharacter::OnMoveRight(float Value) {
	if (Value != 0.0f) {
		if (IsFlying) AddMovementInput(FirstPersonCameraComponent->GetRightVector(), Value);
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFirstPersonCharacter::OnStickMove(FVector2D Input) {
	if (Input.Size() != 0.0f) {
		FVector2D ValidInput = UInputUtility::AccommodateDeadzone(Input, STICK_MOVE_DEADZONE);
		if (IsFlying) {
			AddMovementInput(FirstPersonCameraComponent->GetForwardVector(), ValidInput.Y);
			AddMovementInput(FirstPersonCameraComponent->GetRightVector(), ValidInput.X);
		} else {
			AddMovementInput(GetActorForwardVector(), ValidInput.Y);
			AddMovementInput(GetActorRightVector(), ValidInput.X);
		}
	}
}

void AFirstPersonCharacter::OnStickLook(FVector2D Input) {
	FVector2D Rotation = GamepadLookAdapter->calculatePlayerRotation(Input, GetWorld()->GetDeltaSeconds());

	AddControllerYawInput(Rotation.X);
	AddControllerPitchInput(Rotation.Y);
}

void AFirstPersonCharacter::OnCrouchPress() {
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Crouch Press");
}

void AFirstPersonCharacter::OnCrouchRelease() {
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Crouch Release");
}

void AFirstPersonCharacter::OnJumpPress() {
	Jump();
}

void AFirstPersonCharacter::OnJumpRelease() {
	StopJumping();
}

void AFirstPersonCharacter::OnRunPress() {
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Run Press");
}

void AFirstPersonCharacter::OnRunRelease() {
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Run Release");
}

void AFirstPersonCharacter::OnFlyPress() {
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Fly Press");
}

void AFirstPersonCharacter::OnFlyRelease() {
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Fly Release");
}

void AFirstPersonCharacter::OnStickLookX(float Input) {
	CurrentLookInput.X = Input;
}

void AFirstPersonCharacter::OnStickLookY(float Input) {
	CurrentLookInput.Y = Input;
}