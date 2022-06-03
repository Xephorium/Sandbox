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
		OnStickMove(CurrentMoveInput);
	}
}


/*--- Input Setup Functions ---*/

void AFirstPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &AFirstPersonCharacter::OnStickMoveX);
	PlayerInputComponent->BindAxis("MoveForward", this, &AFirstPersonCharacter::OnStickMoveY);
	PlayerInputComponent->BindAxis("StickLookRight", this, &AFirstPersonCharacter::OnStickLookX);
	PlayerInputComponent->BindAxis("StickLookUp", this, &AFirstPersonCharacter::OnStickLookY);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFirstPersonCharacter::OnFaceBottomPress);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFirstPersonCharacter::OnFaceBottomRelease);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AFirstPersonCharacter::SetupSteamInputComponent() {
	SteamInputComponent->SetupSteamInput();

	SteamInputComponent->BindStickLeft(this, FName("OnStickMove"));
	SteamInputComponent->BindStickRight(this, FName("OnStickLook"));
	SteamInputComponent->BindTriggerLeft(this, FName("OnTriggerLeft"));
	SteamInputComponent->BindTriggerRight(this, FName("OnTriggerRight"));
	SteamInputComponent->BindStickLeftPress(this, FName("OnStickLeftPress"));
	SteamInputComponent->BindStickRightPress(this, FName("OnStickRightPress"));
	SteamInputComponent->BindStartPress(this, FName("OnStartPress"));
	SteamInputComponent->BindEndPress(this, FName("OnEndPress"));
	SteamInputComponent->BindFaceTopPress(this, FName("OnFaceTopPress"));
	SteamInputComponent->BindFaceLeftPress(this, FName("OnFaceLeftPress"));
	SteamInputComponent->BindFaceRightPress(this, FName("OnFaceRightPress"));
	SteamInputComponent->BindFaceBottomPress(this, FName("OnFaceBottomPress"));
	SteamInputComponent->BindFaceBottomRelease(this, FName("OnFaceBottomRelease"));
	SteamInputComponent->BindBumperLeftPress(this, FName("OnBumperLeftPress"));
	SteamInputComponent->BindBumperRightPress(this, FName("OnBumperRightPress"));
	SteamInputComponent->BindDPadUpPress(this, FName("OnDPadUpPress"));
	SteamInputComponent->BindDPadLeftPress(this, FName("OnDPadLeftPress"));
	SteamInputComponent->BindDPadRightPress(this, FName("OnDPadRightPress"));
	SteamInputComponent->BindDPadDownPress(this, FName("OnDPadDownPress"));
}


/*--- Input Handlong Functions ---*/

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

void AFirstPersonCharacter::OnTriggerRight(float Input) {
	if (GEngine && Input > 0.5f) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, FString::SanitizeFloat(Input));
}

void AFirstPersonCharacter::OnTriggerLeft(float Input) {
	if (GEngine && Input > 0.5f) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, FString::SanitizeFloat(Input));
}

void AFirstPersonCharacter::OnStickLeftPress() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Stick Left");
}

void AFirstPersonCharacter::OnStickRightPress() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Stick Right");
}

void AFirstPersonCharacter::OnStartPress() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Start");
}

void AFirstPersonCharacter::OnEndPress() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "End");
}

void AFirstPersonCharacter::OnFaceTopPress() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Y");
}

void AFirstPersonCharacter::OnFaceLeftPress() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "X");
}

void AFirstPersonCharacter::OnFaceRightPress() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "B");
}

void AFirstPersonCharacter::OnFaceBottomPress() {
	Jump();
}

void AFirstPersonCharacter::OnFaceBottomRelease() {
	StopJumping();
}

void AFirstPersonCharacter::OnBumperLeftPress() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Bumper Left");
}

void AFirstPersonCharacter::OnBumperRightPress() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Bumper Right");
}

void AFirstPersonCharacter::OnDPadUpPress() {
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Up");
}

void AFirstPersonCharacter::OnDPadLeftPress() {
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Left");
}

void AFirstPersonCharacter::OnDPadRightPress() {
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Right");
}

void AFirstPersonCharacter::OnDPadDownPress() {
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Down");
}


/*--- Unreal Input State Functions ---*/

void AFirstPersonCharacter::OnStickMoveX(float Value) {
	CurrentMoveInput.X = Value;
}

void AFirstPersonCharacter::OnStickMoveY(float Value) {
	CurrentMoveInput.Y = Value;
}

void AFirstPersonCharacter::OnStickLookX(float Input) {
	CurrentLookInput.X = Input;
}

void AFirstPersonCharacter::OnStickLookY(float Input) {
	CurrentLookInput.Y = Input;
}