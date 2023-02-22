
#include "SteamInputComponent.h"
#include "AllLevels/Input/GamepadType.h"
#include <iostream>
#include <string>

/*
 *  SteamInputComponent.cpp                          Chris Cruzen
 *  Sandbox                                            05.29.2022
 *
 *    SteamInputComponent is a utility class that delegates input
 *  Delegates from the SteamInput library to bound game functions.
 * 
 *  Note: SteamInput requires all possible game inputs be defined
 *        in <steam install>/controller_config/game_actions_#.vfd,
 *        where # is the steam app ID. Until an actual steam app
 *        ID is registered, 480 will work with debug builds. Once
 *        the game is ready to ship, steamworks will let you
 *        choose a game_action_manifest file to upload, which
 *        contains the same settings and will be installed on
 *        each user's system.
 */


/*--- Lifecycle Functions ---*/

void USteamInputComponent::OnTick(float DeltaTime) {
	if (IsSteamInputAvailable()) {
		CheckForConnectedControllers(); // Checks for Connected Controllers
		SteamInput()->RunFrame(); // Queries Steam for Updated Inputs

		// Read First Controller Input
		if (controllers[0]) {

			// Delegate Stick Input
			DelegateStickInput(GetAnalogInput("StickLeft"), StickLeftDelegate);
			DelegateYInvertedStickInput(GetAnalogInput("StickRight"), StickRightDelegate);

			// Delegate Trigger Input
			DelegateTriggerInput(GetAnalogInput("TriggerLeft"), TriggerLeftDelegate);
			DelegateTriggerInput(GetAnalogInput("TriggerRight"), TriggerRightDelegate);

			// Delegate Stick Click Input
			DelegateButtonInput(GetDigitalInput("StickLeftClick"), IsStickLeftPressed, StickLeftPressDelegate, StickLeftReleaseDelegate);
			DelegateButtonInput(GetDigitalInput("StickRightClick"), IsStickRightPressed, StickRightPressDelegate, StickRightReleaseDelegate);

			// Delegate Start/End Input
			DelegateButtonInput(GetDigitalInput("Start"), IsStartPressed, StartPressDelegate, StartReleaseDelegate);
			DelegateButtonInput(GetDigitalInput("End"), IsEndPressed, EndPressDelegate, EndReleaseDelegate);

			// Delegate Face Button Inputs
			DelegateButtonInput(GetDigitalInput("FaceTop"), IsFaceTopPressed, FaceTopPressDelegate, FaceTopReleaseDelegate);
			DelegateButtonInput(GetDigitalInput("FaceLeft"), IsFaceLeftPressed, FaceLeftPressDelegate, FaceLeftReleaseDelegate);
			DelegateButtonInput(GetDigitalInput("FaceRight"), IsFaceRightPressed, FaceRightPressDelegate, FaceRightReleaseDelegate);
			DelegateButtonInput(GetDigitalInput("FaceBottom"), IsFaceBottomPressed, FaceBottomPressDelegate, FaceBottomReleaseDelegate);

			// Delegate Bumper Input
			DelegateButtonInput(GetDigitalInput("BumperLeft"), IsBumperLeftPressed, BumperLeftPressDelegate, BumperLeftReleaseDelegate);
			DelegateButtonInput(GetDigitalInput("BumperRight"), IsBumperRightPressed, BumperRightPressDelegate, BumperRightReleaseDelegate);

			// Delegate Directional Pad Input
			DelegateButtonInput(GetDigitalInput("DPadUp"), IsDPadUpPressed, DPadUpPressDelegate, DPadUpReleaseDelegate);
			DelegateButtonInput(GetDigitalInput("DPadLeft"), IsDPadLeftPressed, DPadLeftPressDelegate, DPadLeftReleaseDelegate);
			DelegateButtonInput(GetDigitalInput("DPadRight"), IsDPadRightPressed, DPadRightPressDelegate, DPadRightReleaseDelegate);
			DelegateButtonInput(GetDigitalInput("DPadDown"), IsDPadDownPressed, DPadDownPressDelegate, DPadDownReleaseDelegate);

		}
	}
}


/*--- Action Binding Functions ---*/

void USteamInputComponent::BindStickLeft(UObject * InUserObject, const FName & InFunctionName) {
	StickLeftDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindStickRight(UObject * InUserObject, const FName & InFunctionName) {
	StickRightDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindTriggerLeft(UObject * InUserObject, const FName & InFunctionName) {
	TriggerLeftDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindTriggerRight(UObject * InUserObject, const FName & InFunctionName) {
	TriggerRightDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindStickLeftPress(UObject * InUserObject, const FName & InFunctionName) {
	StickLeftPressDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindStickLeftRelease(UObject * InUserObject, const FName & InFunctionName) {
	StickLeftReleaseDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindStickRightPress(UObject * InUserObject, const FName & InFunctionName) {
	StickRightPressDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindStickRightRelease(UObject * InUserObject, const FName & InFunctionName) {
	StickRightReleaseDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindStartPress(UObject * InUserObject, const FName & InFunctionName) {
	StartPressDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindStartRelease(UObject * InUserObject, const FName & InFunctionName) {
	StartReleaseDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindEndPress(UObject * InUserObject, const FName & InFunctionName) {
	EndPressDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindEndRelease(UObject * InUserObject, const FName & InFunctionName) {
	EndReleaseDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindFaceTopPress(UObject * InUserObject, const FName & InFunctionName) {
	FaceTopPressDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindFaceTopRelease(UObject * InUserObject, const FName & InFunctionName) {
	FaceTopReleaseDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindFaceLeftPress(UObject * InUserObject, const FName & InFunctionName) {
	FaceLeftPressDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindFaceLeftRelease(UObject * InUserObject, const FName & InFunctionName) {
	FaceLeftReleaseDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindFaceRightPress(UObject * InUserObject, const FName & InFunctionName) {
	FaceRightPressDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindFaceRightRelease(UObject * InUserObject, const FName & InFunctionName) {
	FaceRightReleaseDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindFaceBottomPress(UObject * InUserObject, const FName & InFunctionName) {
	FaceBottomPressDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindFaceBottomRelease(UObject * InUserObject, const FName & InFunctionName) {
	FaceBottomReleaseDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindDPadUpPress(UObject * InUserObject, const FName & InFunctionName) {
	DPadUpPressDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindDPadUpRelease(UObject * InUserObject, const FName & InFunctionName) {
	DPadUpReleaseDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindDPadLeftPress(UObject * InUserObject, const FName & InFunctionName) {
	DPadLeftPressDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindDPadLeftRelease(UObject * InUserObject, const FName & InFunctionName) {
	DPadLeftReleaseDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindDPadRightPress(UObject * InUserObject, const FName & InFunctionName) {
	DPadRightPressDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindDPadRightRelease(UObject * InUserObject, const FName & InFunctionName) {
	DPadRightReleaseDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindDPadDownPress(UObject * InUserObject, const FName & InFunctionName) {
	DPadDownPressDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindDPadDownRelease(UObject * InUserObject, const FName & InFunctionName) {
	DPadDownReleaseDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindBumperLeftPress(UObject * InUserObject, const FName & InFunctionName) {
	BumperLeftPressDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindBumperLeftRelease(UObject * InUserObject, const FName & InFunctionName) {
	BumperLeftReleaseDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindBumperRightPress(UObject * InUserObject, const FName & InFunctionName) {
	BumperRightPressDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindBumperRightRelease(UObject * InUserObject, const FName & InFunctionName) {
	BumperRightReleaseDelegate.BindUFunction(InUserObject, InFunctionName);
}


/*--- Event Binding Functions ---*/

void USteamInputComponent::BindControllerConnect(UObject * InUserObject, const FName & InFunctionName) {
	ControllerConnectDelegate.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindControllerDisconnect(UObject * InUserObject, const FName & InFunctionName) {
	ControllerDisconnectDelegate.BindUFunction(InUserObject, InFunctionName);
}


/*--- Steam API Functions ---*/

void USteamInputComponent::SetupSteamInput() {

	InitializeSteamInput();
	if (IsSteamInputAvailable()) {
		 // Refresh Input
		SteamInput()->RunFrame();
		CheckForConnectedControllers();
	}
}

void USteamInputComponent::InitializeSteamInput() {
	if (SteamInput() != nullptr)
		if (SteamInput()->Init(false)) IsSteamInputInitialized = true;
		else IsSteamInputInitialized = false;
	else IsSteamInputInitialized = false;
}

bool USteamInputComponent::IsSteamInputAvailable() {
	if (IsSteamInputInitialized && SteamInput() != nullptr) return true;
	else return false;
}

void USteamInputComponent::CheckForConnectedControllers() {
	// Activate Sandbox Action Set
	SandboxSetHandle = SteamInput()->GetActionSetHandle("SandboxControls");

	// Get List of Connected Controllers
	controllers = new InputHandle_t[STEAM_INPUT_MAX_COUNT];
	SteamInput()->GetConnectedControllers(controllers);

	if (controllers[0]) {

		// Activate First Person Action Set
		SteamInput()->ActivateActionSet(controllers[0], SandboxSetHandle);

		// Handle Connect
		if (!WasControllerConnected) {
			if (ControllerConnectDelegate.IsBound()) ControllerConnectDelegate.Execute();
			WasControllerConnected = true;
		}
	
	} else {

		// Handle Disconnect
		if (WasControllerConnected) {
			if (ControllerDisconnectDelegate.IsBound()) ControllerDisconnectDelegate.Execute();
			WasControllerConnected = false;
		}
	}
}

EGamepadType USteamInputComponent::GetFirstConnectedGamepadType() {
	if (SteamInput()) {
		if (controllers[0]) {
			ESteamInputType InputType = SteamInput()->GetInputTypeForHandle(controllers[0]);

			switch (InputType) {
				case ESteamInputType::k_ESteamInputType_SteamController:
					return EGamepadType::Steam;
				case ESteamInputType::k_ESteamInputType_SteamDeckController:
					return EGamepadType::SteamDeck;
				case ESteamInputType::k_ESteamInputType_XBox360Controller:
					return EGamepadType::Xbox360;
				case ESteamInputType::k_ESteamInputType_XBoxOneController:
					return EGamepadType::XboxOne;
				case ESteamInputType::k_ESteamInputType_PS3Controller:
					return EGamepadType::PlayStation3;
				case ESteamInputType::k_ESteamInputType_PS4Controller:
					return EGamepadType::PlayStation4;
				case ESteamInputType::k_ESteamInputType_PS5Controller:
					return EGamepadType::PlayStation5;
				case ESteamInputType::k_ESteamInputType_SwitchProController:
					return EGamepadType::SwitchPro;
				case ESteamInputType::k_ESteamInputType_SwitchJoyConSingle:
					return EGamepadType::SwitchJoyConSingle;
				case ESteamInputType::k_ESteamInputType_SwitchJoyConPair:
					return EGamepadType::SwitchJoyConPair;
				default:
					return EGamepadType::GenericSteam;
			}
		}
	}

	return EGamepadType::Disconnected;
}

InputDigitalActionData_t USteamInputComponent::GetDigitalInput(char* name) {
	return SteamInput()->GetDigitalActionData(controllers[0], SteamInput()->GetDigitalActionHandle(name));
}

InputAnalogActionData_t USteamInputComponent::GetAnalogInput(char* name) {
	return SteamInput()->GetAnalogActionData(controllers[0], SteamInput()->GetAnalogActionHandle(name));
}


/*--- Input Delegation Functions ---*/

void USteamInputComponent::DelegateButtonInput(
		InputDigitalActionData_t Action,
		bool &IsPressed,
		InputDelegate DelegatePress,
		InputDelegate DelegateRelease
	) {
		if (Action.bState && !IsPressed) {
			IsPressed = true;
			if (DelegatePress.IsBound()) DelegatePress.Execute();
		} else if (!Action.bState && IsPressed) {
			IsPressed = false;
			if (DelegateRelease.IsBound()) DelegateRelease.Execute();
		}
}

void USteamInputComponent::DelegateTriggerInput(InputAnalogActionData_t Action, FloatInputDelegate Delegate) {
	if (Delegate.IsBound()) Delegate.Execute(Action.x);
}

void USteamInputComponent::DelegateStickInput(InputAnalogActionData_t Action, VectorInputDelegate Delegate) {
	if (Delegate.IsBound()) Delegate.Execute(FVector2D(Action.x, Action.y));
}

void USteamInputComponent::DelegateYInvertedStickInput(InputAnalogActionData_t Action, VectorInputDelegate Delegate) {
	if (Delegate.IsBound()) Delegate.Execute(FVector2D(Action.x, -Action.y));
}