
#include "InputUtility.h"

/*
 *  InputUtility.cpp                                  Chris Cruzen
 *  Sandbox                                             06.01.2022
 * 
 *   InputUtility.cpp is a helper class that houses common gamepad
 *  input processing functionality.
 */


// Discards input beneath STICK_LOOK_DEADZONE, scaling valid input
// within the remaining [0,1] range.
FVector2D UInputUtility::AccommodateDeadzone(FVector2D Input, float Deadzone) {
    if (Input.Size() < Deadzone) return FVector2D::ZeroVector;
    else return Input.GetSafeNormal() * ((Input.Size() - Deadzone) / (1.0f - Deadzone));
}