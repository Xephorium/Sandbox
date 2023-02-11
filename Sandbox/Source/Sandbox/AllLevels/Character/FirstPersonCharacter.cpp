
#include "FirstPersonCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "ControllerDiagnosticWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GrabComponent.h"
#include "GrabbableComponent.h"
#include "AllLevels/Input/GamepadLookAdapter.h"

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

	SetupGamepadLookAdapter();
	SetupControllerDiagnosticWidget();
	SetupGrabComponent();
}

void AFirstPersonCharacter::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);
}


/*--- Input Setup Functions ---*/

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


/*--- Input Handling Overrides ---*/

void AFirstPersonCharacter::OnMouseHorizontal(float Input) {
	AddControllerYawInput(Input);
}

void AFirstPersonCharacter::OnMouseVertical(float Input) {
	AddControllerPitchInput(Input);
}

void AFirstPersonCharacter::OnStickLeft(FVector2D Input) {
	if (Input.Size() != 0.0f) {
		if (IsFlying) {
			AddMovementInput(FirstPersonCameraComponent->GetForwardVector(), Input.Y);
			AddMovementInput(FirstPersonCameraComponent->GetRightVector(), Input.X);
			AddMovementInput(FirstPersonCameraComponent->GetUpVector(), VerticalForceUp + VerticalForceDown);
		} else {
			AddMovementInput(GetActorForwardVector(), Input.Y);
			AddMovementInput(GetActorRightVector(), Input.X);
		}
	}
}

void AFirstPersonCharacter::OnStickRight(FVector2D Input) {
	FVector2D Rotation = GamepadLookAdapter->calculatePlayerRotation(Input, GetWorld()->GetDeltaSeconds());

	AddControllerYawInput(Rotation.X);
	AddControllerPitchInput(Rotation.Y);
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