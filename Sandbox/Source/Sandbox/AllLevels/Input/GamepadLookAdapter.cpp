
#include <math.h>
#include <cmath>
#include "AllLevels/Input/InputUtility.h"
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
    FVector2D ValidInput = UInputUtility::AccommodateDeadzone(Input, STICK_LOOK_DEADZONE);
    IsInTurnZone(Input);
    return FVector2D(ValidInput.X, ValidInput.Y * STICK_LOOK_HORIZ_MULTIPLIER);
}


/*--- Private Functions ---*/

bool UGamepadLookAdapter::IsInTurnZone(FVector2D Input) {

    // Look magnitude must be > STICK_TURN_THRESHOLD.
    bool MagnitudeCheck = abs(Input.X) >= abs((Input.GetSafeNormal() * STICK_TURN_THRESHOLD).X)
        && Input.GetSafeNormal() != FVector2D::ZeroVector;

    // Look angle must be < STICK_TURN_FALLOFF_ANGLE.
    float RadiansToDegrees = (360.0f / (PI * 2.0f));
    float Angle = abs(atan2(Input.Y, Input.X) * RadiansToDegrees);
    bool angleCheck = Angle <= STICK_TURN_FALLOFF_ANGLE;

    return MagnitudeCheck && angleCheck;
}
