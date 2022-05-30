
#include "SteamInputComponent.h"

/*
 *  SteamInputComponent.cpp                          Chris Cruzen
 *  Sandbox                                            05.29.2022
 *
 *    SteamInputComponent is a utility class that delegates input
 *  events from the SteamInput library to bound game functions.
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
			InputDigitalActionData_t JumpAction = ReadDigitalActionData("Jump");

			// Delegate Move Input
			MoveForwardEvent.Execute(MoveAction.y);
			MoveRightEvent.Execute(MoveAction.x);

			// Delegate Look Input
			LookEvent.Execute(LookAction.x, LookAction.y);

			// Delegate Jump Input
			if (JumpAction.bState && !IsJumpPressed) {
				IsJumpPressed = true;
				JumpPressEvent.Execute();
			} else if (!JumpAction.bState && IsJumpPressed) {
				IsJumpPressed = false;
				JumpReleaseEvent.Execute();
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

void USteamInputComponent::BindJumpPress(UObject * InUserObject, const FName & InFunctionName) {
	JumpPressEvent.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindJumpRelease(UObject * InUserObject, const FName & InFunctionName) {
	JumpReleaseEvent.BindUFunction(InUserObject, InFunctionName);
	//JumpPressEvent.Execute(); // Call On SteamInput Event
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
