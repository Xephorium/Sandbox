
#include "GamepadLookAdapter.h"
#include <math.h>
#include <cmath>
#include "AllLevels/Input/InputUtility.h"

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

FVector2D UGamepadLookAdapter::calculatePlayerRotation(FVector2D Input, float TimeDelta) {
    FVector2D ValidInput = UInputUtility::AccommodateDeadzone(Input, STICK_LOOK_DEADZONE);
    float TurnFactor = CalculateTurnFactor(Input, TimeDelta);
    float InputDirection = ValidInput.X / abs(ValidInput.X);

    return FVector2D(
        ValidInput.X + (InputDirection * TurnFactor * STICK_TURN_STRENGTH),
        ValidInput.Y * STICK_LOOK_HORIZ_MULTIPLIER
    ) * STICK_LOOK_SPEED * TimeDelta;
}


/*--- Private Functions ---*/

float UGamepadLookAdapter::CalculateTurnFactor(FVector2D Input, float TimeDelta) {
    if (IsInTurnZone(Input)) {
        TargetTurnFactor = GetRadialFalloff(Input);

        if (CurrentTurnFactor > TargetTurnFactor) {
            CurrentTurnFactor = TargetTurnFactor;
        } else {
            CurrentTurnFactor += (TargetTurnFactor * (1.0f / STICK_TURN_ACCELERATION_LENGTH)) *  TimeDelta;
        }

    } else {
        TargetTurnFactor = 0.0f;
        CurrentTurnFactor = 0.0f;
    }

    return FMath::Sin((CurrentTurnFactor * PI)/2);
}

bool UGamepadLookAdapter::IsInTurnZone(FVector2D Input) {

    // Look magnitude must be > STICK_TURN_THRESHOLD.
    bool MagnitudeCheck = Input.Size() > STICK_TURN_THRESHOLD;

    // Look angle must be < STICK_TURN_FALLOFF_ANGLE.
    float Angle = abs(atan(Input.Y / Input.X) * RAD2DEG);
    bool angleCheck = Angle <= STICK_TURN_FALLOFF_ANGLE;

    return MagnitudeCheck && angleCheck;
}

float UGamepadLookAdapter::GetRadialFalloff(FVector2D Input) {

    // Turn experiences radial falloff as input strays from vector [±1,0].
    float Angle = abs(atan(Input.Y / Input.X) * RAD2DEG);
    float RadialPercent = FMath::Clamp(1.0f - (Angle / STICK_TURN_FALLOFF_ANGLE), 0.0f, 1.0f);

    // Boost output near the horizon for quicker horizontal turns
    return FMath::Sin((RadialPercent * PI)/2);
}
