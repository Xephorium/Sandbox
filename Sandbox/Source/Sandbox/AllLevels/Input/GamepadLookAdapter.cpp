
#include "GamepadLookAdapter.h"
#include <math.h>
#include <cmath>
#include "AllLevels/Input/InputUtility.h"
#include "AllLevels/Utility/LogUtility.h"

/*
 *  GamepadLookAdapter.cpp                           Chris Cruzen
 *  Sandbox                                            06.04.2022
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
 *  magnitude than STICK_TURN_THRESHOLD along the right and left edges
 *  of the stick assumes the player's intention to "Turn" and accelerates
 *  the player's rotation. The turn strength is also clamped to a radial
 *  falloff, where the turn speed decreases as input strays from the
 *  horizontal axis.
 */


/*--- Primary Player Rotation Function ---*/

FVector2D UGamepadLookAdapter::calculatePlayerRotation(FVector2D Input, float TimeDelta) {

    // Disregard Input Within Deadzone
    FVector2D ValidInput = UInputUtility::AccommodateDeadzone(Input, STICK_DEADZONE);
    
    // Apply Easing Curve
    FVector2D ScaledInput = ValidInput * ((UGamepadLookAdapter::CircleEaseIn(ValidInput.Size()) + ValidInput.Size()) / 2.0f);
    
    // Calculate Turn Strength & Direction
    float TurnFactor = CalculateTurnFactor(ScaledInput, TimeDelta);
    float InputDirection = ScaledInput.X / abs(ScaledInput.X);

    // Build Final Rotation Vector
    return FVector2D(
        ScaledInput.X + (InputDirection * TurnFactor * STICK_TURN_STRENGTH),
        ScaledInput.Y * STICK_LOOK_VERTICAL_MULTIPLIER
    ) * STICK_LOOK_SPEED * TimeDelta;
}


/*--- Calculation Functions ---*/

/* Returns a float [0,1] representing the strenth of the camera's current turn,
 * which gradually increases the longer look magnitude > STICK_TURN_THRESHOLD.
 */
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
    bool AngleCheck = Angle <= STICK_TURN_FALLOFF_ANGLE;

    return MagnitudeCheck && AngleCheck;
}

/* Returns a float [0,1] representing how close to the horizontal axis the
 * current stick input is. When the stick is directly left or right, the 
 * returned value is 1. When the stick is STICK_TURN_FALLOFF_ANGLE above or
 * below the axis, the returned value is 0.
 */
float UGamepadLookAdapter::GetRadialFalloff(FVector2D Input) {
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

float UGamepadLookAdapter::CircleEaseIn(float Input) {
    return 1 - sqrtf(1 - FMath::Min(Input * Input, 1.0f));
}