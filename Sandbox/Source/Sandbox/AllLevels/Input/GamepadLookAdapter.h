
#pragma once

#include "GamepadLookAdapter.generated.h"

/*
 *  GamepadLookAdapter.h                            Chris Cruzen
 *  Sandbox                                           05.31.2022
 *
 *  Header file for GamepadLookAdapter.cpp.
 */

UCLASS()
class UGamepadLookAdapter : public UObject {
	
	GENERATED_BODY()


	/*--- Variables ---*/

	private: const float RAD2DEG = (360.0f / (PI * 2.0f));

	private: const float STICK_DEADZONE = 0.19f;
	private: const float STICK_LOOK_SPEED = 40.0f;
	private: const float STICK_TURN_STRENGTH = 1.20f;
	private: const float STICK_TURN_THRESHOLD = 0.975f;
	private: const float STICK_TURN_FALLOFF_ANGLE = 55.0f;
	private: const float STICK_TURN_ACCELERATION_LENGTH = 0.65f;

	private: const float STICK_LOOK_VERTICAL_MULTIPLIER = 0.77f;

	private: const float TURN_ACCELERATION_LENGTH = 0.8f;
	private: float TargetTurnFactor = 0.0f;
	private: float CurrentTurnFactor = 0.0f;


	/*--- Primary Player Rotation Function ---*/

	/** Calculates absolute 2D vector representing camera rotation from current player state given stick input.
	 *  Input: Normalized stick input (x=[0,1], y=[0,1])
	 *  Output: Absolute camera rotation for given frame (x=[0,infinity], y=[0,infinity])
	 */
	public: FVector2D calculatePlayerRotation(FVector2D Input, float TimeDelta);


	/*--- Calculation Functions ---*/

	/* Returns a float [0,1] representing the strenth of the camera's current turn,
     * which gradually increases the longer look magnitude > STICK_TURN_THRESHOLD.
     */
	private: float CalculateTurnFactor(FVector2D Input, float TimeDelta);

	private: bool IsInTurnZone(FVector2D Input);

	/* Returns a float [0,1] representing how close to the horizontal axis the
 	 * current stick input is. When the stick is directly left or right, the 
 	 * returned value is 1. When the stick is STICK_TURN_FALLOFF_ANGLE above or
 	 * below the axis, the returned value is 0.
	 */
	private: float GetRadialFalloff(FVector2D Input);


	/*--- Interpolation Functions ---*/

	private: float CubicEaseIn(float Input);

	private: float CubicEaseOut(float Input);

	private: float SinEaseIn(float Input);

	private: float SinEaseOut(float Input);

	private: float CircleEaseIn(float Input);
};

