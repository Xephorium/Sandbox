
#include "InputCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "ControllerDiagnosticWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputSettings.h"
#include "GrabComponent.h"
#include "GrabbableComponent.h"
#include "AllLevels/Input/InputUtility.h"
#include "AllLevels/Utility/LogUtility.h"
#include "AllLevels/Input/GamepadLookAdapter.h"
#include "Dependencies/Steam/SteamInputComponent.h"

/*
 *  InputCharacter.cpp                                    Chris Cruzen
 *  Sandbox                                                 02.11.2023
 *
 *    InputCharacter is one of Sandbox's most base character classes.
 *  It reads all input events from both Steam and Unreal Engine before
 *  delegating to more specific character subclasses.
 */


AInputCharacter::AInputCharacter() {

	// Setup Capsule & Camera
	GetCapsuleComponent()->InitCapsuleSize(DEFAULT_CAPSULE_RADIUS, DEFAULT_CAPSULE_HEIGHT);
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, DEFAULT_EYE_HEIGHT));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Configure Movement
	IsFlying = false;
	GetCharacterMovement()->JumpZVelocity = DEFAULT_JUMP_VELOCITY;
	GetCharacterMovement()->AirControl = DEFAULT_AIR_CONTROL;
}

void AInputCharacter::BeginPlay() {
	Super::BeginPlay();

	SetupSteamInputComponent();
	SetupGamepadLookAdapter();
	SetupControllerDiagnosticWidget();
	SetupGrabComponent();
}

void AInputCharacter::Tick(float DeltaSeconds) {
	if (SteamInputComponent->IsSteamInputAvailable()) {
		SteamInputComponent->OnTick(DeltaSeconds);
	} else {
		OnStickLook(CurrentLookInput);
	}
	OnStickMove(CurrentMoveInput);
}


/*--- Input Setup Functions ---*/

// APawn Override
void AInputCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &AInputCharacter::OnStickMoveX);
	PlayerInputComponent->BindAxis("MoveForward", this, &AInputCharacter::OnStickMoveY);
	PlayerInputComponent->BindAxis("StickLookRight", this, &AInputCharacter::OnStickLookX); // Controller X
	PlayerInputComponent->BindAxis("StickLookUp", this, &AInputCharacter::OnStickLookY);    // Controller Y
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);                  // Mouse X
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);              // Mouse Y
	PlayerInputComponent->BindAxis("TriggerLeft", this, &AInputCharacter::OnTriggerLeft);
	PlayerInputComponent->BindAxis("TriggerRight", this, &AInputCharacter::OnTriggerRight);
	PlayerInputComponent->BindAction("Start", IE_Pressed, this, &AInputCharacter::OnStartPress);
	PlayerInputComponent->BindAction("End", IE_Released, this, &AInputCharacter::OnEndPress);
	PlayerInputComponent->BindAction("FaceTop", IE_Pressed, this, &AInputCharacter::OnFaceTopPress);
	PlayerInputComponent->BindAction("FaceLeft", IE_Pressed, this, &AInputCharacter::OnFaceLeftPress);
	PlayerInputComponent->BindAction("FaceRight", IE_Pressed, this, &AInputCharacter::OnFaceRightPress);
	PlayerInputComponent->BindAction("FaceBottom", IE_Pressed, this, &AInputCharacter::OnFaceBottomPress);
	PlayerInputComponent->BindAction("FaceBottom", IE_Released, this, &AInputCharacter::OnFaceBottomRelease);
	PlayerInputComponent->BindAction("BumperLeft", IE_Pressed, this, &AInputCharacter::OnBumperLeftPress);
	PlayerInputComponent->BindAction("BumperLeft", IE_Released, this, &AInputCharacter::OnBumperLeftRelease);
	PlayerInputComponent->BindAction("BumperRight", IE_Pressed, this, &AInputCharacter::OnBumperRightPress);
	PlayerInputComponent->BindAction("BumperRight", IE_Released, this, &AInputCharacter::OnBumperRightRelease);
	PlayerInputComponent->BindAction("DPadUp", IE_Pressed, this, &AInputCharacter::OnDPadUpPress);
	PlayerInputComponent->BindAction("DPadLeft", IE_Pressed, this, &AInputCharacter::OnDPadLeftPress);
	PlayerInputComponent->BindAction("DPadRight", IE_Pressed, this, &AInputCharacter::OnDPadRightPress);
	PlayerInputComponent->BindAction("DPadDown", IE_Pressed, this, &AInputCharacter::OnDPadDownPress);
}

void AInputCharacter::SetupSteamInputComponent() {
	SteamInputComponent = NewObject<USteamInputComponent>(this);
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
	SteamInputComponent->BindBumperLeftRelease(this, FName("OnBumperLeftRelease"));
	SteamInputComponent->BindBumperRightPress(this, FName("OnBumperRightPress"));
	SteamInputComponent->BindBumperRightRelease(this, FName("OnBumperRightRelease"));
	SteamInputComponent->BindDPadUpPress(this, FName("OnDPadUpPress"));
	SteamInputComponent->BindDPadLeftPress(this, FName("OnDPadLeftPress"));
	SteamInputComponent->BindDPadRightPress(this, FName("OnDPadRightPress"));
	SteamInputComponent->BindDPadDownPress(this, FName("OnDPadDownPress"));

	SteamInputComponent->BindControllerConnect(this, FName("OnControllerConnected"));
	SteamInputComponent->BindControllerDisconnect(this, FName("OnControllerDisconnected"));
}

void AInputCharacter::SetupGamepadLookAdapter() {
	GamepadLookAdapter = NewObject<UGamepadLookAdapter>(this);
}

void AInputCharacter::SetupControllerDiagnosticWidget() {
	// Nothing... Yet.
}

void AInputCharacter::SetupGrabComponent() {
	UPhysicsHandleComponent *PhysicsComponent = NewObject<UPhysicsHandleComponent>(this);
	PhysicsComponent->RegisterComponent();
	GrabComponent = NewObject<UGrabComponent>(this);
	GrabComponent->RegisterComponent();
}


/*--- Input Handlong Functions ---*/

void AInputCharacter::OnStickMove(FVector2D Input) {
	if (Input.Size() != 0.0f) {
		FVector2D ValidInput = UInputUtility::AccommodateDeadzone(Input, STICK_MOVE_DEADZONE);
		if (IsFlying) {
			AddMovementInput(FirstPersonCameraComponent->GetForwardVector(), ValidInput.Y);
			AddMovementInput(FirstPersonCameraComponent->GetRightVector(), ValidInput.X);
			AddMovementInput(FirstPersonCameraComponent->GetUpVector(), VerticalForceUp + VerticalForceDown);
		} else {
			AddMovementInput(GetActorForwardVector(), ValidInput.Y);
			AddMovementInput(GetActorRightVector(), ValidInput.X);
		}
	}
}

void AInputCharacter::OnStickLook(FVector2D Input) {
	FVector2D Rotation = GamepadLookAdapter->calculatePlayerRotation(Input, GetWorld()->GetDeltaSeconds());

	AddControllerYawInput(Rotation.X);
	AddControllerPitchInput(Rotation.Y);
}

void AInputCharacter::OnTriggerRight(float Input) {
	if (GEngine && Input > 0.5f) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, FString::SanitizeFloat(Input));
}

void AInputCharacter::OnTriggerLeft(float Input) {
	if (GEngine && Input > 0.5f) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, FString::SanitizeFloat(Input));
}

void AInputCharacter::OnStickLeftPress() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Stick Left");
}

void AInputCharacter::OnStickRightPress() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Stick Right");
}

void AInputCharacter::OnStartPress() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Start");
}

void AInputCharacter::OnEndPress() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "End");
}

void AInputCharacter::OnFaceTopPress() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Y");
}

void AInputCharacter::OnFaceLeftPress() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "X");
}

void AInputCharacter::OnFaceRightPress() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "B");
}

void AInputCharacter::OnFaceBottomPress() {
	Jump();
}

void AInputCharacter::OnFaceBottomRelease() {
	StopJumping();
}

void AInputCharacter::OnBumperLeftPress() {
	if (IsFlying) {
		VerticalForceDown = -VERTICAL_FLIGHT_SPEED;
	} else {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Bumper Left");
	}
}

void AInputCharacter::OnBumperLeftRelease() {
	VerticalForceDown = 0.0f;
}

void AInputCharacter::OnBumperRightPress() {
	if (IsFlying) {
		VerticalForceUp = VERTICAL_FLIGHT_SPEED;
	} else {
		if (IsGrabEnabled) {
			GrabComponent->GrabObject();
		}
	}
}

void AInputCharacter::OnBumperRightRelease() {
	if (IsFlying) {
		VerticalForceUp = 0.0f;
	} else {
		if (IsGrabEnabled) {
			GrabComponent->ReleaseObject();
		}
	}
	
}

void AInputCharacter::OnDPadUpPress() {
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Up");
}

void AInputCharacter::OnDPadLeftPress() {
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Left");
}

void AInputCharacter::OnDPadRightPress() {
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Right");
}

void AInputCharacter::OnDPadDownPress() {
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Down");
}


/*--- Event Handling Functions ---*/

void AInputCharacter::OnControllerConnected() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Controller Connected");
}

void AInputCharacter::OnControllerDisconnected() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Controller Disconnected");
}


/*--- Unreal Input State Functions ---*/

void AInputCharacter::OnStickMoveX(float Value) {
	CurrentMoveInput.X = Value;
}

void AInputCharacter::OnStickMoveY(float Value) {
	CurrentMoveInput.Y = Value;
}

void AInputCharacter::OnStickLookX(float Input) {
	CurrentLookInput.X = Input;
}

void AInputCharacter::OnStickLookY(float Input) {
	CurrentLookInput.Y = Input;
}