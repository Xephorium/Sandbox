// Copyright Epic Games, Inc. All Rights Reserved.

#include "FirstPersonCharacter.h"
#include "FirstPersonProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "AllLevels/Utility/LogUtility.h"
#include "Dependencies/SteamInputComponent.h"

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

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 0.0f, 58.5f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	
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

	PlayerInputComponent->BindAxis("MoveForward", this, &AFirstPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFirstPersonCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &AFirstPersonCharacter::MoveUp);
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

	SteamInputComponent->BindJumpPress(this, FName("OnJumpPress"));
	SteamInputComponent->BindJumpRelease(this, FName("OnJumpRelease"));
}


/*--- Input Handlong Functions ---*/

void AFirstPersonCharacter::MoveForward(float Value) {
	if (Value != 0.0f) {
		if (IsFlying) AddMovementInput(FirstPersonCameraComponent->GetForwardVector(), Value);
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFirstPersonCharacter::MoveRight(float Value) {
	if (Value != 0.0f) {
		if (IsFlying) AddMovementInput(FirstPersonCameraComponent->GetRightVector(), Value);
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFirstPersonCharacter::MoveUp(float Value) {
	if (Value != 0.0f) {
		if (IsFlying) AddMovementInput(FirstPersonCameraComponent->GetUpVector(), Value);
		else AddMovementInput(GetActorUpVector(), Value);
	}
}

void AFirstPersonCharacter::OnJumpPress() {
	Jump();
}

void AFirstPersonCharacter::OnJumpRelease() {
	StopJumping();
}


void AFirstPersonCharacter::TurnAtRate(float Rate) {
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFirstPersonCharacter::LookUpAtRate(float Rate) {
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}