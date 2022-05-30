
#include "SteamInputComponent.h"

/*
 *  SteamInputComponent.cpp                          Chris Cruzen
 *  Sandbox                                            05.29.2022
 *
 *    SteamInputComponent is a utility class that delegates input
 *  events from the SteamInput library to bound game functions.
 */


/*--- Lifecycle ---*/

void USteamInputComponent::OnTick(float DeltaTime) {
	if (IsSteamInputAvailable()) {
		SteamInput()->RunFrame(); // Queries Steam for Updated Inputs

		// Read First Controller Input
		if (controllers[0]) {

			// Read Action Data
			InputDigitalActionData_t JumpAction = ReadDigitalActionData("Jump");

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


/*--- Action Binding ---*/

void USteamInputComponent::BindJumpPress(UObject * InUserObject, const FName & InFunctionName) {
	JumpPressEvent.BindUFunction(InUserObject, InFunctionName);
}

void USteamInputComponent::BindJumpRelease(UObject * InUserObject, const FName & InFunctionName) {
	JumpReleaseEvent.BindUFunction(InUserObject, InFunctionName);
	//JumpPressEvent.Execute(); // Call On SteamInput Event
}



/*--- Steam API ---*/

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
	if (SteamInput())
		if (SteamInput()->Init(false)) IsSteamInputInitialized = true;
		else IsSteamInputInitialized = false;
	else IsSteamInputInitialized = false;
}

bool USteamInputComponent::IsSteamInputAvailable() {
	if (IsSteamInputInitialized && SteamInput()) return true;
	else return false;
}

InputDigitalActionData_t USteamInputComponent::ReadDigitalActionData(char* name) {
	return SteamInput()->GetDigitalActionData(controllers[0], SteamInput()->GetDigitalActionHandle(name));
}
