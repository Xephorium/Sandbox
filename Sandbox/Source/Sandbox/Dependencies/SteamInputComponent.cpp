
#include <iostream>
#include <string>
#include "SteamInputComponent.h"

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
			InputAnalogActionData_t MoveAction = ReadAnalogActionData("Move");
			InputAnalogActionData_t LookAction = ReadAnalogActionData("Camera");
			InputDigitalActionData_t CrouchAction = ReadDigitalActionData("Crouch");
			InputDigitalActionData_t JumpAction = ReadDigitalActionData("Jump");
			InputDigitalActionData_t RunAction = ReadDigitalActionData("Run");
			InputDigitalActionData_t FlyAction = ReadDigitalActionData("Fly");

			// Delegate Move Input
			if (MoveForwardEvent.IsBound()) MoveForwardEvent.Execute(MoveAction.y);
			if (MoveRightEvent.IsBound()) MoveRightEvent.Execute(MoveAction.x);

			// Delegate Look Input
			if (LookEvent.IsBound()) LookEvent.Execute(LookAction.x, LookAction.y);

			// Delegate Crouch Input
			if (CrouchAction.bState && !IsCrouchPressed) {
				IsCrouchPressed = true;
				if (CrouchPressEvent.IsBound()) CrouchPressEvent.Execute();
			} else if (!CrouchAction.bState && IsCrouchPressed) {
				IsCrouchPressed = false;
				if (CrouchReleaseEvent.IsBound()) CrouchReleaseEvent.Execute();
			}

			// Delegate Jump Input
			if (JumpAction.bState && !IsJumpPressed) {
				IsJumpPressed = true;
				if (JumpPressEvent.IsBound()) JumpPressEvent.Execute();
			} else if (!JumpAction.bState && IsJumpPressed) {
				IsJumpPressed = false;
				if (JumpReleaseEvent.IsBound()) JumpReleaseEvent.Execute();
			}

			// Delegate Run Input
			if (RunAction.bState && !IsRunPressed) {
				IsRunPressed = true;
				if (RunPressEvent.IsBound()) RunPressEvent.Execute();
			} else if (!RunAction.bState && IsRunPressed) {
				IsRunPressed = false;
				if (RunReleaseEvent.IsBound()) RunReleaseEvent.Execute();
			}

			// Delegate Fly Input
			if (FlyAction.bState && !IsFlyPressed) {
				IsFlyPressed = true;
				if (FlyPressEvent.IsBound()) FlyPressEvent.Execute();
			} else if (!FlyAction.bState && IsFlyPressed) {
				IsFlyPressed = false;
				if (FlyReleaseEvent.IsBound()) FlyReleaseEvent.Execute();
			}
		}
	}
}


/*--- Action Binding Functions ---*/

void USteamInputComponent::BindMoveForward(UObject * InUserObject, const FName & InFunctionName) {
	MoveForwardEvent.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindMoveRight(UObject * InUserObject, const FName & InFunctionName) {
	MoveRightEvent.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindLook(UObject * InUserObject, const FName & InFunctionName) {
	LookEvent.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindCrouchPress(UObject * InUserObject, const FName & InFunctionName) {
	CrouchPressEvent.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindCrouchRelease(UObject * InUserObject, const FName & InFunctionName) {
	CrouchReleaseEvent.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindJumpPress(UObject * InUserObject, const FName & InFunctionName) {
	JumpPressEvent.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindJumpRelease(UObject * InUserObject, const FName & InFunctionName) {
	JumpReleaseEvent.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindRunPress(UObject * InUserObject, const FName & InFunctionName) {
	RunPressEvent.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindRunRelease(UObject * InUserObject, const FName & InFunctionName) {
	RunReleaseEvent.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindFlyPress(UObject * InUserObject, const FName & InFunctionName) {
	FlyPressEvent.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindFlyRelease(UObject * InUserObject, const FName & InFunctionName) {
	FlyReleaseEvent.BindUFunction(InUserObject, InFunctionName);
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
