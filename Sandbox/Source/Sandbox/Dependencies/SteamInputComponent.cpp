
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

		// Query Steam for Updated Inputs
		SteamInput()->RunFrame();

		// Read Controller Input
		if (ConnectedSteamControllers[0]) {
			InputDigitalActionData_t JumpAction = SteamInput()->GetDigitalActionData(
				ConnectedSteamControllers[0],
				SteamInput()->GetDigitalActionHandle("Jump")
			);

			if (JumpAction.bState) {
				GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::White, "Jump!");
			}
		}
	}
}


/*--- Action Binding Functions ---*/

void USteamInputComponent::BindJumpPress(UObject * InUserObject, const FName & InFunctionName) {
	JumpPressEvent.BindUFunction(InUserObject, InFunctionName);
	JumpPressEvent.Execute(); // Call On SteamInput Event
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
		ConnectedSteamControllers = new InputHandle_t[STEAM_INPUT_MAX_COUNT];
		SteamInput()->GetConnectedControllers(ConnectedSteamControllers);

		// Activate First Person Action Set
		if (ConnectedSteamControllers[0]) {
			SteamInput()->ActivateActionSet(ConnectedSteamControllers[0], FirstPersonSetHandle);
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
