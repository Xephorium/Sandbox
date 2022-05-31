// Copyright Epic Games, Inc. All Rights Reserved.

#include "FirstPersonCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "AllLevels/Utility/LogUtility.h"
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
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	IsFlying = false;

	// Create SteamInputComponent
	SteamInputComponent = CreateDefaultSubobject<USteamInputComponent>(TEXT("SteamInputComponent"));
	
}

void AFirstPersonCharacter::BeginPlay() {
	Super::BeginPlay();

	SetupSteamInputComponent();
}

void AFirstPersonCharacter::Tick(float DeltaSeconds) {
	SteamInputComponent->OnTick(DeltaSeconds);
}


/*--- Input Setup Functions ---*/

void AFirstPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFirstPersonCharacter::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFirstPersonCharacter::OnMoveRight);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFirstPersonCharacter::OnJumpPress);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFirstPersonCharacter::OnJumpRelease);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFirstPersonCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFirstPersonCharacter::LookUpAtRate);
}

void AFirstPersonCharacter::SetupSteamInputComponent() {
	SteamInputComponent->SetupSteamInput();

	SteamInputComponent->BindMoveForward(this, FName("OnMoveForward"));
	SteamInputComponent->BindMoveRight(this, FName("OnMoveRight"));
	SteamInputComponent->BindLook(this, FName("OnLook"));
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

void AFirstPersonCharacter::OnLook(float ValueX, float ValueY) {
	FVector2D Input = FVector2D(ValueX / 90.0f, ValueY / 90.0f);
	TurnAtRate(Input.X);
	LookUpAtRate(Input.Y);
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


void AFirstPersonCharacter::TurnAtRate(float Rate) {
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFirstPersonCharacter::LookUpAtRate(float Rate) {
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}