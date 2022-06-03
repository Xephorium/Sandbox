
#include "SteamInputComponent.h"
#include <iostream>
#include <string>

/*
 *  SteamInputComponent.cpp                          Chris Cruzen
 *  Sandbox                                            05.29.2022
 *
 *    SteamInputComponent is a utility class that delegates input
 *  events from the SteamInput library to bound game functions.
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
		SteamInput()->RunFrame(); // Queries Steam for Updated Inputs

		// Read First Controller Input
		if (controllers[0]) {

			// Read Action Data
			InputAnalogActionData_t StickLeftAction = ReadAnalogActionData("Move");
			InputAnalogActionData_t StickRightAction = ReadAnalogActionData("Camera");
			InputDigitalActionData_t StickLeftClickAction = ReadDigitalActionData("Crouch");
			InputDigitalActionData_t FaceBottomAction = ReadDigitalActionData("Jump");
			InputDigitalActionData_t BumperLeftAction = ReadDigitalActionData("Run");
			InputDigitalActionData_t DirectionalUpAction = ReadDigitalActionData("Fly");

			// Delegate Left Stick Input
			if (StickLeftEvent.IsBound()) StickLeftEvent.Execute(FVector2D(StickLeftAction.x, StickLeftAction.y));

			// Delegate Right Stick Input
			if (StickRightEvent.IsBound()) StickRightEvent.Execute(FVector2D(StickRightAction.x, -StickRightAction.y));

			// Delegate Left Stick Click Input
			if (StickLeftClickAction.bState && !IsStickLeftPressed) {
				IsStickLeftPressed = true;
				if (StickLeftPressEvent.IsBound()) StickLeftPressEvent.Execute();
			} else if (!StickLeftClickAction.bState && IsStickLeftPressed) {
				IsStickLeftPressed = false;
				if (StickLeftReleaseEvent.IsBound()) StickLeftReleaseEvent.Execute();
			}

			// Delegate Face Button Bottom Input
			if (FaceBottomAction.bState && !IsFaceBottomPressed) {
				IsFaceBottomPressed = true;
				if (FaceBottomPressEvent.IsBound()) FaceBottomPressEvent.Execute();
			} else if (!FaceBottomAction.bState && IsFaceBottomPressed) {
				IsFaceBottomPressed = false;
				if (FaceBottomReleaseEvent.IsBound()) FaceBottomReleaseEvent.Execute();
			}

			// Delegate Run Input
			if (BumperLeftAction.bState && !IsBumperLeftPressed) {
				IsBumperLeftPressed = true;
				if (BumperLeftPressEvent.IsBound()) BumperLeftPressEvent.Execute();
			} else if (!BumperLeftAction.bState && IsBumperLeftPressed) {
				IsBumperLeftPressed = false;
				if (BumperLeftReleaseEvent.IsBound()) BumperLeftReleaseEvent.Execute();
			}

			// Delegate Fly Input
			if (DirectionalUpAction.bState && !IsDirectionalUpPressed) {
				IsDirectionalUpPressed = true;
				if (DirectionalUpPressEvent.IsBound()) DirectionalUpPressEvent.Execute();
			} else if (!DirectionalUpAction.bState && IsDirectionalUpPressed) {
				IsDirectionalUpPressed = false;
				if (DirectionalUpReleaseEvent.IsBound()) DirectionalUpReleaseEvent.Execute();
			}
		}
	}
}


/*--- Action Binding Functions ---*/

void USteamInputComponent::BindStickLeft(UObject * InUserObject, const FName & InFunctionName) {
	StickLeftEvent.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindStickRight(UObject * InUserObject, const FName & InFunctionName) {
	StickRightEvent.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindStickLeftPress(UObject * InUserObject, const FName & InFunctionName) {
	StickLeftPressEvent.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindStickLeftRelease(UObject * InUserObject, const FName & InFunctionName) {
	StickLeftReleaseEvent.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindStickRightPress(UObject * InUserObject, const FName & InFunctionName) {
	StickRightPressEvent.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindStickRightRelease(UObject * InUserObject, const FName & InFunctionName) {
	StickRightReleaseEvent.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindFaceBottomPress(UObject * InUserObject, const FName & InFunctionName) {
	FaceBottomPressEvent.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindFaceBottomRelease(UObject * InUserObject, const FName & InFunctionName) {
	FaceBottomReleaseEvent.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindBumperLeftPress(UObject * InUserObject, const FName & InFunctionName) {
	BumperLeftPressEvent.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindBumperLeftRelease(UObject * InUserObject, const FName & InFunctionName) {
	BumperLeftReleaseEvent.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindDirectionalUpPress(UObject * InUserObject, const FName & InFunctionName) {
	DirectionalUpPressEvent.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindDirectionalUpRelease(UObject * InUserObject, const FName & InFunctionName) {
	DirectionalUpReleaseEvent.BindUFunction(InUserObject, InFunctionName);
}



/*--- Steam API Functions ---*/

void USteamInputComponent::SetupSteamInput() {

	InitializeSteamInput();
	if (IsSteamInputAvailable()) {

		// Print Status
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::White, "Steam Input - Active");

		// Refresh Input
		SteamInput()->RunFrame();

		// Activate First Person Action Set
		InputActionSetHandle_t FirstPersonSetHandle = SteamInput()->GetActionSetHandle("FirstPersonControls");

		// Get List of Connected Controllers
		controllers = new InputHandle_t[STEAM_INPUT_MAX_COUNT];	
		SteamInput()->GetConnectedControllers(controllers);

		// Activate First Person Action Set
		if (controllers[0]) {
			SteamInput()->ActivateActionSet(controllers[0], FirstPersonSetHandle);
		}

	} else {

		// Print Status
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::White, "Steam Input - Unavailable (Reverting to UE4 Input)");
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

InputDigitalActionData_t USteamInputComponent::ReadDigitalActionData(char* name) {
	return SteamInput()->GetDigitalActionData(controllers[0], SteamInput()->GetDigitalActionHandle(name));
}

InputAnalogActionData_t USteamInputComponent::ReadAnalogActionData(char* name) {
	return SteamInput()->GetAnalogActionData(controllers[0], SteamInput()->GetAnalogActionHandle(name));
}