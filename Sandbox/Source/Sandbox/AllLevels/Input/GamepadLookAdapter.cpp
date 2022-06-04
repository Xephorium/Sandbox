
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
    FVector2D ValidInput = UInputUtility::AccommodateDeadzone(Input, STICK_DEADZONE);
    FVector2D ScaledInput = ValidInput * ((UGamepadLookAdapter::SinEaseIn(ValidInput.Size()) + ValidInput.Size()) / 2.0f);
    float TurnFactor = CalculateTurnFactor(ScaledInput, TimeDelta);
    float InputDirection = ScaledInput.X / abs(ScaledInput.X);

    return FVector2D(
        ScaledInput.X + (InputDirection * TurnFactor * STICK_TURN_STRENGTH),
        ScaledInput.Y * STICK_LOOK_VERTICAL_MULTIPLIER
    ) * STICK_LOOK_SPEED * TimeDelta;
}


/*--- Calculation Functions ---*/

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

    return CurrentTurnFactor;
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

    return RadialPercent;
}


/*--- Interpolation Functions ---*/

float UGamepadLookAdapter::CubicEaseIn(float Input) {
    return Input * Input;
}

float UGamepadLookAdapter::CubicEaseOut(float Input) {
    float Param = Input - 1.0f;
    return Param * Param * Param + 1.0f;
}

float UGamepadLookAdapter::SinEaseIn(float Input) {
    return FMath::Sin((Input - 1.f) * (PI / 2.f)) + 1.f;
}

float UGamepadLookAdapter::SinEaseOut(float Input) {
    return FMath::Sin((Input * PI)/2);
}