
#include "InputCharacter.h"
#include "AllLevels/Input/InputUtility.h"
#include "Components/InputComponent.h"
#include "ControllerDiagnosticWidget.h"
#include "Dependencies/Steam/SteamInputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"

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
	PlayerInputComponent->BindAction("StickLeftClick", IE_Released, this, &AInputCharacter::OnStickLeftRelease);
	PlayerInputComponent->BindAction("StickRightClick", IE_Pressed, this, &AInputCharacter::OnStickRightPress);
	PlayerInputComponent->BindAction("StickRightClick", IE_Released, this, &AInputCharacter::OnStickRightRelease);
	PlayerInputComponent->BindAction("Start", IE_Pressed, this, &AInputCharacter::OnStartPress);
	PlayerInputComponent->BindAction("Start", IE_Released, this, &AInputCharacter::OnStartRelease);
	PlayerInputComponent->BindAction("End", IE_Pressed, this, &AInputCharacter::OnEndPress);
	PlayerInputComponent->BindAction("End", IE_Released, this, &AInputCharacter::OnEndRelease);
	PlayerInputComponent->BindAction("FaceTop", IE_Pressed, this, &AInputCharacter::OnFaceTopPress);
	PlayerInputComponent->BindAction("FaceTop", IE_Released, this, &AInputCharacter::OnFaceTopRelease);
	PlayerInputComponent->BindAction("FaceLeft", IE_Pressed, this, &AInputCharacter::OnFaceLeftPress);
	PlayerInputComponent->BindAction("FaceLeft", IE_Released, this, &AInputCharacter::OnFaceLeftRelease);
	PlayerInputComponent->BindAction("FaceRight", IE_Pressed, this, &AInputCharacter::OnFaceRightPress);
	PlayerInputComponent->BindAction("FaceRight", IE_Released, this, &AInputCharacter::OnFaceRightRelease);
	PlayerInputComponent->BindAction("FaceBottom", IE_Pressed, this, &AInputCharacter::OnFaceBottomPress);
	PlayerInputComponent->BindAction("FaceBottom", IE_Released, this, &AInputCharacter::OnFaceBottomRelease);
	PlayerInputComponent->BindAction("BumperLeft", IE_Pressed, this, &AInputCharacter::OnBumperLeftPress);
	PlayerInputComponent->BindAction("BumperLeft", IE_Released, this, &AInputCharacter::OnBumperLeftRelease);
	PlayerInputComponent->BindAction("BumperRight", IE_Pressed, this, &AInputCharacter::OnBumperRightPress);
	PlayerInputComponent->BindAction("BumperRight", IE_Released, this, &AInputCharacter::OnBumperRightRelease);
	PlayerInputComponent->BindAction("DPadUp", IE_Pressed, this, &AInputCharacter::OnDPadUpPress);
	PlayerInputComponent->BindAction("DPadUp", IE_Released, this, &AInputCharacter::OnDPadUpRelease);
	PlayerInputComponent->BindAction("DPadLeft", IE_Pressed, this, &AInputCharacter::OnDPadLeftPress);
	PlayerInputComponent->BindAction("DPadLeft", IE_Released, this, &AInputCharacter::OnDPadLeftRelease);
	PlayerInputComponent->BindAction("DPadRight", IE_Pressed, this, &AInputCharacter::OnDPadRightPress);
	PlayerInputComponent->BindAction("DPadRight", IE_Released, this, &AInputCharacter::OnDPadRightRelease);
	PlayerInputComponent->BindAction("DPadDown", IE_Pressed, this, &AInputCharacter::OnDPadDownPress);
	PlayerInputComponent->BindAction("DPadDown", IE_Released, this, &AInputCharacter::OnDPadDownRelease);
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
	SteamInputComponent->BindStickLeftRelease(this, FName("OnStickLeftRelease"));
	SteamInputComponent->BindStickRightPress(this, FName("OnStickRightPress"));
	SteamInputComponent->BindStickRightRelease(this, FName("OnStickRightRelease"));
	SteamInputComponent->BindStartPress(this, FName("OnStartPress"));
	SteamInputComponent->BindStartRelease(this, FName("OnStartRelease"));
	SteamInputComponent->BindEndPress(this, FName("OnEndPress"));
	SteamInputComponent->BindEndRelease(this, FName("OnEndRelease"));
	SteamInputComponent->BindFaceTopPress(this, FName("OnFaceTopPress"));
	SteamInputComponent->BindFaceTopRelease(this, FName("OnFaceTopRelease"));
	SteamInputComponent->BindFaceLeftPress(this, FName("OnFaceLeftPress"));
	SteamInputComponent->BindFaceLeftRelease(this, FName("OnFaceLeftRelease"));
	SteamInputComponent->BindFaceRightPress(this, FName("OnFaceRightPress"));
	SteamInputComponent->BindFaceRightRelease(this, FName("OnFaceRightRelease"));
	SteamInputComponent->BindFaceBottomPress(this, FName("OnFaceBottomPress"));
	SteamInputComponent->BindFaceBottomRelease(this, FName("OnFaceBottomRelease"));
	SteamInputComponent->BindBumperLeftPress(this, FName("OnBumperLeftPress"));
	SteamInputComponent->BindBumperLeftRelease(this, FName("OnBumperLeftRelease"));
	SteamInputComponent->BindBumperRightPress(this, FName("OnBumperRightPress"));
	SteamInputComponent->BindBumperRightRelease(this, FName("OnBumperRightRelease"));
	SteamInputComponent->BindDPadUpPress(this, FName("OnDPadUpPress"));
	SteamInputComponent->BindDPadUpRelease(this, FName("OnDPadUpRelease"));
	SteamInputComponent->BindDPadLeftPress(this, FName("OnDPadLeftPress"));
	SteamInputComponent->BindDPadLeftRelease(this, FName("OnDPadLeftRelease"));
	SteamInputComponent->BindDPadRightPress(this, FName("OnDPadRightPress"));
	SteamInputComponent->BindDPadRightRelease(this, FName("OnDPadRightRelease"));
	SteamInputComponent->BindDPadDownPress(this, FName("OnDPadDownPress"));
	SteamInputComponent->BindDPadDownRelease(this, FName("OnDPadDownRelease"));

	SteamInputComponent->BindControllerConnect(this, FName("OnControllerConnected"));
	SteamInputComponent->BindControllerDisconnect(this, FName("OnControllerDisconnected"));
}


/*--- Diagnostic Functions ---*/

bool AInputCharacter::CheckForControllerDiagnosticWidget() {
	TArray<UUserWidget*> FoundWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(
		GetWorld(),
		FoundWidgets,
		UControllerDiagnosticWidget::StaticClass()
	);

	if (FoundWidgets.Num() > 0 && Cast<UControllerDiagnosticWidget>(FoundWidgets[0])) {
		ControllerDiagnosticWidget = Cast<UControllerDiagnosticWidget>(FoundWidgets[0]);
	}

	return ControllerDiagnosticWidget != nullptr;
}


/*--- Overridable Input Handling Functions ---*/

void AInputCharacter::OnMouseHorizontal(float Input) {
	if (IsDebugLoggingEnabled && GEngine && Input > 0.0f) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, "Mouse X: " + FString::SanitizeFloat(Input));
}

void AInputCharacter::OnMouseVertical(float Input) {
	if (IsDebugLoggingEnabled && GEngine && Input > 0.0f) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, "Mouse Y: " + FString::SanitizeFloat(Input));
}

void AInputCharacter::OnStickLeft(FVector2D Input) {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnStickLeft(Input);
}

void AInputCharacter::OnStickRight(FVector2D Input) {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnStickRight(Input);
}

void AInputCharacter::OnTriggerLeft(float Input) {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnTriggerLeft(Input);
	if (IsDebugLoggingEnabled && GEngine && Input > 0.0f) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, FString::SanitizeFloat(Input));
}

void AInputCharacter::OnTriggerRight(float Input) {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnTriggerRight(Input);
	if (IsDebugLoggingEnabled && GEngine && Input > 0.0f) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, FString::SanitizeFloat(Input));
}

void AInputCharacter::OnStickLeftPress() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnStickLeftPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Stick Left Press");
}

void AInputCharacter::OnStickLeftRelease() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnStickLeftRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Stick Left Release");
}

void AInputCharacter::OnStickRightPress() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnStickRightPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Stick Right Press");
}

void AInputCharacter::OnStickRightRelease() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnStickRightRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Stick Right Release");
}

void AInputCharacter::OnStartPress() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnStartPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Start Press");
}

void AInputCharacter::OnStartRelease() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnStartRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Start Release");
}

void AInputCharacter::OnEndPress() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnEndPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "End Press");
}

void AInputCharacter::OnEndRelease() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnEndRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "End Release");
}

void AInputCharacter::OnFaceTopPress() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnFaceTopPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Y Press");
}

void AInputCharacter::OnFaceTopRelease() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnFaceTopRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Y Release");
}

void AInputCharacter::OnFaceLeftPress() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnFaceLeftPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "X Press");
}

void AInputCharacter::OnFaceLeftRelease() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnFaceLeftRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "X Release");
}

void AInputCharacter::OnFaceRightPress() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnFaceRightPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "B Press");
}

void AInputCharacter::OnFaceRightRelease() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnFaceRightRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "B Release");
}

void AInputCharacter::OnFaceBottomPress() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnFaceBottomPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "A Press");
}

void AInputCharacter::OnFaceBottomRelease() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnFaceBottomRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "A Release");
}

void AInputCharacter::OnBumperLeftPress() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnBumperLeftPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Bumper Left Press");
}

void AInputCharacter::OnBumperLeftRelease() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnBumperLeftRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Bumper Left Release");
}

void AInputCharacter::OnBumperRightPress() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnBumperRightPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Bumper Right Press");
}

void AInputCharacter::OnBumperRightRelease() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnBumperRightRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Bumper Right Release");
}

void AInputCharacter::OnDPadUpPress() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnDPadUpPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Up Press");
}

void AInputCharacter::OnDPadUpRelease() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnDPadUpRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Up Release");
}

void AInputCharacter::OnDPadLeftPress() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnDPadLeftPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Left Press");
}

void AInputCharacter::OnDPadLeftRelease() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnDPadLeftRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Left Release");
}

void AInputCharacter::OnDPadRightPress() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnDPadRightPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Right Press");
}

void AInputCharacter::OnDPadRightRelease() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnDPadRightRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Right Release");
}

void AInputCharacter::OnDPadDownPress() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnDPadDownPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Down Press");
}

void AInputCharacter::OnDPadDownRelease() {
	if (IsControllerDiagnosticEnabled && CheckForControllerDiagnosticWidget()) ControllerDiagnosticWidget->OnDPadDownRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Down Release");
}



/*--- Overridable Event Handling Functions ---*/

void AInputCharacter::OnControllerConnected() {
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Controller Connected");
}

void AInputCharacter::OnControllerDisconnected() {
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Controller Disconnected");
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