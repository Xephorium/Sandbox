
#include <iostream>
#include <string>
#include <cmath>
#include "AllLevels/Utility/LogUtility.h"
#include "GamepadLookAdapter.h"

/*
 *  GamepadLookAdapter.cpp                           Chris Cruzen
 *  Sandbox                                            05.31.2022
 * 
 *  This class contains the logic required to translate analog toggle stick
 *  input to satisfying first person look motion. It does this by treating
 *  input differently based on which of two zones it activates: "Look" or
 *  "Turn".
 * 
 *                               _.-""""-._
 *                             .'\        /`.
 *                            /   :      :   \
 *                           |   /        \   |
 *                    Turn --->  :  Look   : <--- Turn
 *                           |   \        /   |
 *                            \   :      :   /
 *                             `./        \.'
 *                                `-....-'
 *
 *  "Look" simply rotates the camera based on an acceleration curve.
 *  This area makes up the majority of the stick. However, input of greater
 *  magnitude than _______________________ along the right and left edges
 *  of the stick assumes the player's intention to "Turn" and accelerates
 *  the player's rotation. The turn strength is also clamped to a radial
 *  falloff, where the turn speed decreases as input strays from the
 *  horizontal axis.
 */


/*--- Primary Player Rotation Function ---*/

FVector2D UGamepadLookAdapter::calculatePlayerRotation(FVector2D Input) {
    FVector2D ValidInput = AccommodateDeadzone(Input);
    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, FString::SanitizeFloat(ValidInput.Size()));
    return FVector2D(ValidInput.X, ValidInput.Y * STICK_LOOK_HORIZ_MULTIPLIER);
}


/*--- Private Functions ---*/

bool UGamepadLookAdapter::IsInTurnZone(FVector2D Input) {
    return Input.Size() > STICK_TURN_THRESHOLD;
}

// Discards input beneath STICK_LOOK_DEADZONE, finessing all other valid vectors into a [0,1] range.
FVector2D UGamepadLookAdapter::AccommodateDeadzone(FVector2D Input) {
    float Scale = 1.0f / (1.0f - STICK_LOOK_DEADZONE);
    return FVector2D(
        FMath::Clamp((abs(Input.X) - STICK_LOOK_DEADZONE) * Scale, 0.0f, 1.0f) * (Input.X / abs(Input.X)),
        FMath::Clamp((abs(Input.Y) - STICK_LOOK_DEADZONE) * Scale, 0.0f, 1.0f) * (Input.Y / abs(Input.Y))
    );
}