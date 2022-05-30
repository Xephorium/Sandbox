
#include "SteamInputComponent.h"

/*
 *  SteamInputComponent.cpp                          Chris Cruzen
 *  Sandbox                                            05.29.2022
 *
 *    SteamInputComponent is a utility class that delegates input
 *  events from the SteamInput library to bound game functions.
 */


/*--- Lifecycle Functions ---*/

USteamInputComponent::USteamInputComponent() {}


/*--- Action Binding Functions ---*/

void USteamInputComponent::BindJumpPress(UObject * InUserObject, const FName & InFunctionName) {
	JumpPressEvent.BindUFunction(InUserObject, InFunctionName);
	JumpPressEvent.Execute(); // Call On SteamInput Event
}

