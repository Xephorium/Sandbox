
#include "InputCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "AllLevels/Input/InputUtility.h"
#include "Dependencies/Steam/SteamInputComponent.h"

/*
 *  InputCharacter.cpp                                    Chris Cruzen
 *  Sandbox                                                 02.11.2023
 *
 *    InputCharacter is one of Sandbox's most base character classes.
 *  It reads all input events from both Steam and Unreal Engine before
 *  delegating to more specific character subclasses.
 */


/*--- Lifecycle Functions ---*/

AInputCharacter::AInputCharacter() { }

void AInputCharacter::BeginPlay() {
	Super::BeginPlay();
	SetupSteamInputComponent();
}

void AInputCharacter::Tick(float DeltaSeconds) {
	if (SteamInputComponent->IsSteamInputAvailable()) {
		SteamInputComponent->OnTick(DeltaSeconds);
	} else {
		OnStickRightInput(CurrentRightStickInput);
	}
	OnStickLeftInput(CurrentLeftStickInput);
}


/*--- Setup Functions ---*/

// APawn Override
void AInputCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	check(PlayerInputComponent);

	// Unreal Keyboard/Mouse
	PlayerInputComponent->BindAxis("Turn", this, &AInputCharacter::OnMouseHorizontal);
	PlayerInputComponent->BindAxis("LookUp", this, &AInputCharacter::OnMouseVertical);

	// Unreal Controller
	PlayerInputComponent->BindAxis("MoveRight", this, &AInputCharacter::OnStickLeftX);
	PlayerInputComponent->BindAxis("MoveForward", this, &AInputCharacter::OnStickLeftY);
	PlayerInputComponent->BindAxis("StickLookRight", this, &AInputCharacter::OnStickRightX);
	PlayerInputComponent->BindAxis("StickLookUp", this, &AInputCharacter::OnStickRightY);
	PlayerInputComponent->BindAxis("TriggerLeft", this, &AInputCharacter::OnTriggerLeft);
	PlayerInputComponent->BindAxis("TriggerRight", this, &AInputCharacter::OnTriggerRight);
	PlayerInputComponent->BindAction("StickLeftClick", IE_Pressed, this, &AInputCharacter::OnStickLeftPress);
	PlayerInputComponent->BindAction("StickRightClick", IE_Pressed, this, &AInputCharacter::OnStickRightPress);
	PlayerInputComponent->BindAction("Start", IE_Pressed, this, &AInputCharacter::OnStartPress);
	PlayerInputComponent->BindAction("End", IE_Pressed, this, &AInputCharacter::OnEndPress);
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

	// Steam Controller
	SteamInputComponent->BindStickLeft(this, FName("OnStickLeftInput"));
	SteamInputComponent->BindStickRight(this, FName("OnStickRightInput"));
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


/*--- Overridable Input Handling Functions ---*/

void AInputCharacter::OnMouseHorizontal(float Input) {
	if (GEngine && Input > 0.0f) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, "Mouse X: " + FString::SanitizeFloat(Input));
}

void AInputCharacter::OnMouseVertical(float Input) {
	if (GEngine && Input > 0.0f) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, "Mouse Y: " + FString::SanitizeFloat(Input));
}

void AInputCharacter::OnStickLeft(FVector2D Input) {
	if (Input.Size() != 0.0f) {
		FVector2D ValidInput = UInputUtility::AccommodateDeadzone(Input, STICK_LEFT_DEADZONE);
	}
}

void AInputCharacter::OnStickRight(FVector2D Input) { 
	if (Input.Size() != 0.0f) {
		FVector2D ValidInput = UInputUtility::AccommodateDeadzone(Input, STICK_RIGHT_DEADZONE);
	}
}

void AInputCharacter::OnTriggerRight(float Input) {
	if (GEngine && Input > 0.0f) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, FString::SanitizeFloat(Input));
}

void AInputCharacter::OnTriggerLeft(float Input) {
	if (GEngine && Input > 0.0f) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, FString::SanitizeFloat(Input));
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
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "A Press");
}

void AInputCharacter::OnFaceBottomRelease() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "A Release");
}

void AInputCharacter::OnBumperLeftPress() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Bumper Left Press");
}

void AInputCharacter::OnBumperLeftRelease() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Bumper Left Release");
}

void AInputCharacter::OnBumperRightPress() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Bumper Right Press");
}

void AInputCharacter::OnBumperRightRelease() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Bumper Right Release");
}

void AInputCharacter::OnDPadUpPress() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Up");
}

void AInputCharacter::OnDPadLeftPress() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Left");
}

void AInputCharacter::OnDPadRightPress() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Right");
}

void AInputCharacter::OnDPadDownPress() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Down");
}


/*--- Overridable Event Handling Functions ---*/

void AInputCharacter::OnControllerConnected() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Controller Connected");
}

void AInputCharacter::OnControllerDisconnected() {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Controller Disconnected");
}


/*--- Private Input Handling Functions ---*/

void AInputCharacter::OnStickLeftX(float Value) {
	CurrentLeftStickInput.X = Value;
}

void AInputCharacter::OnStickLeftY(float Value) {
	CurrentLeftStickInput.Y = Value;
}

void AInputCharacter::OnStickRightX(float Input) {
	CurrentRightStickInput.X = Input;
}

void AInputCharacter::OnStickRightY(float Input) {
	CurrentRightStickInput.Y = Input;
}

void AInputCharacter::OnStickLeftInput(FVector2D Input) {
	if (Input.Size() != 0.0f) {
		OnStickLeft(UInputUtility::AccommodateDeadzone(Input, STICK_LEFT_DEADZONE));
	}
}

void AInputCharacter::OnStickRightInput(FVector2D Input) { 
		if (Input.Size() != 0.0f) {
		OnStickRight(UInputUtility::AccommodateDeadzone(Input, STICK_RIGHT_DEADZONE));
	}
}