// Copyright Epic Games, Inc. All Rights Reserved.

#include "FirstPersonCharacter.h"
#include "FirstPersonProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "../AllLevels/Utility/LogUtility.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AFirstPersonCharacter

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

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
	
	IsFlying = false;
}

void AFirstPersonCharacter::BeginPlay() {

	// Call the base class  
	Super::BeginPlay();

	InitializeSteamInput();
	if (IsSteamInputAvailable()) {

		// Print Status
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::White, "Steam Input - Active");

		// Refresh Input
		SteamInput()->RunFrame();

		// Activate First Person Action Set
		InputActionSetHandle_t FirstPersonSetHandle = SteamInput()->GetActionSetHandle("FirstPersonControls");

		// Get List of Connected Controllers
		ConnectedSteamControllers = new InputHandle_t[STEAM_INPUT_MAX_COUNT];
		SteamInput()->GetConnectedControllers(ConnectedSteamControllers);

		// Activate First Person Action Set
		if (ConnectedSteamControllers[0]) {
			SteamInput()->ActivateActionSet(ConnectedSteamControllers[0], FirstPersonSetHandle);
		}

	} else {

		// Print Status
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::White, "Steam Input - Unavailable (Reverting to UE4 Input)");
	}
}

void AFirstPersonCharacter::Tick(float DeltaSeconds) {
	
	if (IsSteamInputAvailable()) {

		// Query Steam for Updated Inputs
		SteamInput()->RunFrame();

		// Read Controller Input
		if (ConnectedSteamControllers[0]) {
			InputDigitalActionData_t JumpAction = SteamInput()->GetDigitalActionData(
				ConnectedSteamControllers[0],
				SteamInput()->GetDigitalActionHandle("Jump")
			);

			if (JumpAction.bState) {
				GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::White, "Jump!");
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// Input

void AFirstPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {

	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AFirstPersonCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFirstPersonCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &AFirstPersonCharacter::MoveUp);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFirstPersonCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFirstPersonCharacter::LookUpAtRate);
}

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

void AFirstPersonCharacter::TurnAtRate(float Rate) {
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFirstPersonCharacter::LookUpAtRate(float Rate) {
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AFirstPersonCharacter::InitializeSteamInput() {
	if (SteamInput())
		if (SteamInput()->Init(false)) IsSteamInputInitialized = true;
		else IsSteamInputInitialized = false;
	else IsSteamInputInitialized = false;
}

bool AFirstPersonCharacter::IsSteamInputAvailable() {
	if (SteamInput() && IsSteamInputInitialized) return true;
	else return false;
}
