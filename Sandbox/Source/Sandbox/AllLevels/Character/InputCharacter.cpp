
#include "InputCharacter.h"
#include "AllLevels/Input/InputUtility.h"
#include "Components/InputComponent.h"
#include "ControllerDiagnosticWidget.h"
#include "Dependencies/Steam/SteamInputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "UObject/Class.h"

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

	// Process Input
	if (SteamInputComponent->IsSteamInputAvailable()) {
		SteamInputComponent->OnTick(DeltaSeconds);
	} else {
		OnStickLeftInput(NormalizeStickInput(CurrentUnrealStickLeftInput));
		OnStickRightInput(NormalizeStickInput(CurrentUnrealStickRightInput));
	}

	// Handle Controller Diagnostic Display
	if (IsControllerDiagnosticEnabled && IsHoldingToToggleControllerDiagnostic) {
		ToggleControllerDiagnosticCurrentTime += DeltaSeconds;
		if (ToggleControllerDiagnosticCurrentTime >= TOGGLE_CONTROLLER_DIAGNOSTIC_HOLD_TIME) {
			IsHoldingToToggleControllerDiagnostic = false;
			ToggleControllerDiagnosticCurrentTime = 0.0f;
			if (IsControllerDiagnosticShown) {
				HideControllerDiagnosticWidget();
			} else {
				ShowControllerDiagnosticWidget();
			}
			
		}
	}
}


/*--- Setup Functions ---*/

// APawn Override
void AInputCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {
	check(PlayerInputComponent);

	// Unreal Keyboard/Mouse
	PlayerInputComponent->BindAxis("MouseX", this, &AInputCharacter::OnMouseHorizontal);
	PlayerInputComponent->BindAxis("MouseY", this, &AInputCharacter::OnMouseVertical);

	// Unreal Controller
	PlayerInputComponent->BindAxis("StickLeftX", this, &AInputCharacter::OnUnrealStickLeftX);
	PlayerInputComponent->BindAxis("StickLeftY", this, &AInputCharacter::OnUnrealStickLeftY);
	PlayerInputComponent->BindAxis("StickRightX", this, &AInputCharacter::OnUnrealStickRightX);
	PlayerInputComponent->BindAxis("StickRightY", this, &AInputCharacter::OnUnrealStickRightY);
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

void AInputCharacter::ShowControllerDiagnosticWidget() {
	if (IsControllerDiagnosticEnabled && !IsControllerDiagnosticShown) {
		ControllerDiagnosticWidget = CreateWidget<UControllerDiagnosticWidget>(GetWorld(), ControllerDiagnosticWidgetClass);
        ControllerDiagnosticWidget->AddToViewport();
        GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
		IsControllerDiagnosticShown = true;
		UpdatePlatformAndGamepadType();
    }
}

void AInputCharacter::HideControllerDiagnosticWidget() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) {
		IsControllerDiagnosticShown = false;
        ControllerDiagnosticWidget->RemoveFromViewport();
		ControllerDiagnosticWidget = nullptr;
    }
}

void AInputCharacter::UpdatePlatformAndGamepadType() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) {
		ControllerDiagnosticWidget->OnPlatformChange(SteamInputComponent->IsSteamInputAvailable());
		EGamepadType GamepadType = SteamInputComponent->GetFirstConnectedGamepadType();
		switch (GamepadType) {
			case EGamepadType::Generic:
				ControllerDiagnosticWidget->OnInputTypeChange(TEXT("Generic Gamepad"));
				break;
			case EGamepadType::Steam:
				ControllerDiagnosticWidget->OnInputTypeChange(TEXT("Steam Controller"));
				break;
			case EGamepadType::SteamDeck:
				ControllerDiagnosticWidget->OnInputTypeChange(TEXT("Steam Deck Controller"));
				break;
			case EGamepadType::Xbox360:
				ControllerDiagnosticWidget->OnInputTypeChange(TEXT("Xbox 360 Controller"));
				break;
			case EGamepadType::XboxOne:
				ControllerDiagnosticWidget->OnInputTypeChange(TEXT("Xbox One Controller"));
				break;
			case EGamepadType::PlayStation3:
				ControllerDiagnosticWidget->OnInputTypeChange(TEXT("PlayStation 3 Controller"));
				break;
			case EGamepadType::PlayStation4:
				ControllerDiagnosticWidget->OnInputTypeChange(TEXT("PlayStation 4 Controller"));
				break;
			case EGamepadType::PlayStation5:
				ControllerDiagnosticWidget->OnInputTypeChange(TEXT("PlayStation 5 Controller"));
				break;
			case EGamepadType::SwitchPro:
				ControllerDiagnosticWidget->OnInputTypeChange(TEXT("Switch Pro Controller"));
				break;
			case EGamepadType::SwitchJoyConSingle:
				ControllerDiagnosticWidget->OnInputTypeChange(TEXT("Switch Single Joycon"));
				break;
			case EGamepadType::SwitchJoyConPair:
				ControllerDiagnosticWidget->OnInputTypeChange(TEXT("Switch Joycon Pair"));
				break;
			default:
				ControllerDiagnosticWidget->OnInputTypeChange(TEXT("Disconnected"));
		}
	}
}


/*--- Overridable Input Handling Functions ---*/

void AInputCharacter::OnMouseHorizontal(float Input) {
	if (IsDebugLoggingEnabled && GEngine && Input > 0.0f) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, "Mouse X: " + FString::SanitizeFloat(Input));
}

void AInputCharacter::OnMouseVertical(float Input) {
	if (IsDebugLoggingEnabled && GEngine && Input > 0.0f) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, "Mouse Y: " + FString::SanitizeFloat(Input));
}

void AInputCharacter::OnStickLeft(FVector2D Input) {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnStickLeft(Input);
}

void AInputCharacter::OnStickRight(FVector2D Input) {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnStickRight(Input);
}

void AInputCharacter::OnTriggerLeft(float Input) {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnTriggerLeft(Input);
	if (IsDebugLoggingEnabled && GEngine && Input > 0.0f) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, FString::SanitizeFloat(Input));
}

void AInputCharacter::OnTriggerRight(float Input) {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnTriggerRight(Input);
	if (IsDebugLoggingEnabled && GEngine && Input > 0.0f) GEngine->AddOnScreenDebugMessage(-1, 0.5f, FColor::White, FString::SanitizeFloat(Input));
}

void AInputCharacter::OnStickLeftPress() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnStickLeftPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Stick Left Press");
}

void AInputCharacter::OnStickLeftRelease() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnStickLeftRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Stick Left Release");
}

void AInputCharacter::OnStickRightPress() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnStickRightPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Stick Right Press");
}

void AInputCharacter::OnStickRightRelease() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnStickRightRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Stick Right Release");
}

void AInputCharacter::OnStartPress() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnStartPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Start Press");
}

void AInputCharacter::OnStartRelease() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnStartRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Start Release");
}

void AInputCharacter::OnEndPress() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnEndPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "End Press");
}

void AInputCharacter::OnEndRelease() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnEndRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "End Release");
}

void AInputCharacter::OnFaceTopPress() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnFaceTopPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Y Press");
}

void AInputCharacter::OnFaceTopRelease() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnFaceTopRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Y Release");
}

void AInputCharacter::OnFaceLeftPress() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnFaceLeftPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "X Press");
}

void AInputCharacter::OnFaceLeftRelease() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnFaceLeftRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "X Release");
}

void AInputCharacter::OnFaceRightPress() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnFaceRightPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "B Press");
}

void AInputCharacter::OnFaceRightRelease() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnFaceRightRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "B Release");
}

void AInputCharacter::OnFaceBottomPress() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnFaceBottomPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "A Press");
}

void AInputCharacter::OnFaceBottomRelease() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnFaceBottomRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "A Release");
}

void AInputCharacter::OnBumperLeftPress() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnBumperLeftPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Bumper Left Press");
}

void AInputCharacter::OnBumperLeftRelease() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnBumperLeftRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Bumper Left Release");
}

void AInputCharacter::OnBumperRightPress() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnBumperRightPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Bumper Right Press");
}

void AInputCharacter::OnBumperRightRelease() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnBumperRightRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Bumper Right Release");
}

void AInputCharacter::OnDPadUpPress() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnDPadUpPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Up Press");
}

void AInputCharacter::OnDPadUpRelease() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnDPadUpRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Up Release");
}

void AInputCharacter::OnDPadLeftPress() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnDPadLeftPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Left Press");
}

void AInputCharacter::OnDPadLeftRelease() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnDPadLeftRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Left Release");
}

void AInputCharacter::OnDPadRightPress() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnDPadRightPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Right Press");
}

void AInputCharacter::OnDPadRightRelease() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnDPadRightRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Right Release");
}

void AInputCharacter::OnDPadDownPress() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnDPadDownPress();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Down Press");

	IsHoldingToToggleControllerDiagnostic = true;
}

void AInputCharacter::OnDPadDownRelease() {
	if (ControllerDiagnosticWidget && IsControllerDiagnosticShown) ControllerDiagnosticWidget->OnDPadDownRelease();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "D-Pad Down Release");
	
	IsHoldingToToggleControllerDiagnostic = false;
	ToggleControllerDiagnosticCurrentTime = 0.0f;
}



/*--- Overridable Event Handling Functions ---*/

void AInputCharacter::OnControllerConnected() {
	UpdatePlatformAndGamepadType();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Controller Connected");
}

void AInputCharacter::OnControllerDisconnected() {
	UpdatePlatformAndGamepadType();
	if (IsDebugLoggingEnabled && GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Controller Disconnected");
}


/*--- Private Input Handling Functions ---*/

void AInputCharacter::OnUnrealStickLeftX(float Value) {
	CurrentUnrealStickLeftInput.X = Value;
}

void AInputCharacter::OnUnrealStickLeftY(float Value) {
	CurrentUnrealStickLeftInput.Y = Value;
}

void AInputCharacter::OnUnrealStickRightX(float Input) {
	CurrentUnrealStickRightInput.X = Input;
}

void AInputCharacter::OnUnrealStickRightY(float Input) {
	CurrentUnrealStickRightInput.Y = Input;
}

FVector2D AInputCharacter::NormalizeStickInput(FVector2D Input) {
	FVector2D NormalizedInput = Input;
	NormalizedInput.Normalize();
	return NormalizedInput * (FMath::Clamp(Input.Size(), 0.0f, 1.0f) / NormalizedInput.Size());
}

void AInputCharacter::OnStickLeftInput(FVector2D Input) {
	OnStickLeft(UInputUtility::AccommodateDeadzone(Input, STICK_LEFT_DEADZONE));
}

void AInputCharacter::OnStickRightInput(FVector2D Input) { 
	OnStickRight(UInputUtility::AccommodateDeadzone(Input, STICK_RIGHT_DEADZONE));
}