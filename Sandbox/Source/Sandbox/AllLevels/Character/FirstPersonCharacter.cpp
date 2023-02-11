
#include "FirstPersonCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "ControllerDiagnosticWidget.h"
#include "GameFramework/InputSettings.h"
#include "GrabComponent.h"
#include "GrabbableComponent.h"
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

void AFirstPersonCharacter::BeginPlay() {
	Super::BeginPlay();

	SetupSteamInputComponent();
	SetupGamepadLookAdapter();
	SetupControllerDiagnosticWidget();
	SetupGrabComponent();
}

void AFirstPersonCharacter::Tick(float DeltaSeconds) {
	if (SteamInputComponent->IsSteamInputAvailable()) {
		SteamInputComponent->OnTick(DeltaSeconds);
	} else {
		OnStickLook(CurrentLookInput);
	}
	OnStickMove(CurrentMoveInput);
}


/*--- Input Setup Functions ---*/

// APawn Override
void AFirstPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &AFirstPersonCharacter::OnStickMoveX);
	PlayerInputComponent->BindAxis("MoveForward", this, &AFirstPersonCharacter::OnStickMoveY);
	PlayerInputComponent->BindAxis("StickLookRight", this, &AFirstPersonCharacter::OnStickLookX); // Controller X
	PlayerInputComponent->BindAxis("StickLookUp", this, &AFirstPersonCharacter::OnStickLookY);    // Controller Y
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);                  // Mouse X
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);              // Mouse Y
	PlayerInputComponent->BindAxis("TriggerLeft", this, &AFirstPersonCharacter::OnTriggerLeft);
	PlayerInputComponent->BindAxis("TriggerRight", this, &AFirstPersonCharacter::OnTriggerRight);
	PlayerInputComponent->BindAction("Start", IE_Pressed, this, &AFirstPersonCharacter::OnStartPress);
	PlayerInputComponent->BindAction("End", IE_Released, this, &AFirstPersonCharacter::OnEndPress);
	PlayerInputComponent->BindAction("FaceTop", IE_Pressed, this, &AFirstPersonCharacter::OnFaceTopPress);
	PlayerInputComponent->BindAction("FaceLeft", IE_Pressed, this, &AFirstPersonCharacter::OnFaceLeftPress);
	PlayerInputComponent->BindAction("FaceRight", IE_Pressed, this, &AFirstPersonCharacter::OnFaceRightPress);
	PlayerInputComponent->BindAction("FaceBottom", IE_Pressed, this, &AFirstPersonCharacter::OnFaceBottomPress);
	PlayerInputComponent->BindAction("FaceBottom", IE_Released, this, &AFirstPersonCharacter::OnFaceBottomRelease);
	PlayerInputComponent->BindAction("BumperLeft", IE_Pressed, this, &AFirstPersonCharacter::OnBumperLeftPress);
	PlayerInputComponent->BindAction("BumperLeft", IE_Released, this, &AFirstPersonCharacter::OnBumperLeftRelease);
	PlayerInputComponent->BindAction("BumperRight", IE_Pressed, this, &AFirstPersonCharacter::OnBumperRightPress);
	PlayerInputComponent->BindAction("BumperRight", IE_Released, this, &AFirstPersonCharacter::OnBumperRightRelease);
	PlayerInputComponent->BindAction("DPadUp", IE_Pressed, this, &AFirstPersonCharacter::OnDPadUpPress);
	PlayerInputComponent->BindAction("DPadLeft", IE_Pressed, this, &AFirstPersonCharacter::OnDPadLeftPress);
	PlayerInputComponent->BindAction("DPadRight", IE_Pressed, this, &AFirstPersonCharacter::OnDPadRightPress);
	PlayerInputComponent->BindAction("DPadDown", IE_Pressed, this, &AFirstPersonCharacter::OnDPadDownPress);
}

void AFirstPersonCharacter::SetupSteamInputComponent() {
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

void AFirstPersonCharacter::SetupGamepadLookAdapter() {
	GamepadLookAdapter = NewObject<UGamepadLookAdapter>(this);
}

void AFirstPersonCharacter::SetupControllerDiagnosticWidget() {
	// Nothing... Yet.
}

void AFirstPersonCharacter::SetupGrabComponent() {
	UPhysicsHandleComponent *PhysicsComponent = NewObject<UPhysicsHandleComponent>(this);
	PhysicsComponent->RegisterComponent();
	GrabComponent = NewObject<UGrabComponent>(this);
	GrabComponent->RegisterComponent();
}


/*--- Input Handlong Functions ---*/

void AFirstPersonCharacter::OnStickMove(FVector2D Input) {
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
	if (IsFlying) {
		VerticalForceDown = -VERTICAL_FLIGHT_SPEED;
	} else {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Bumper Left");
	}
}

void AFirstPersonCharacter::OnBumperLeftRelease() {
	VerticalForceDown = 0.0f;
}

void AFirstPersonCharacter::OnBumperRightPress() {
	if (IsFlying) {
		VerticalForceUp = VERTICAL_FLIGHT_SPEED;
	} else {
		if (IsGrabEnabled) {
			GrabComponent->GrabObject();
		}
	}
}

void AFirstPersonCharacter::OnBumperRightRelease() {
	if (IsFlying) {
		VerticalForceUp = 0.0f;
	} else {
		if (IsGrabEnabled) {
			GrabComponent->ReleaseObject();
		}
	}
	
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


/*--- Event Handling Functions ---*/

void AFirstPersonCharacter::OnControllerConnected() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Controller Connected");
}

void AFirstPersonCharacter::OnControllerDisconnected() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Controller Disconnected");
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