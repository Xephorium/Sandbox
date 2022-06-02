
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

	private: const float STICK_LOOK_SPEED = 45.0f;
	private: const float STICK_LOOK_HORIZ_FACTOR = 1.17f;
	private: const float STICK_LOOK_VERT_FACTOR = 0.85f;
	private: const float STICK_TURN_THRESHOLD = 0.93f;
	private: const float STICK_TURN_FALLOFF_ANGLE = 67.5f;
	private: const float STICK_TURN_ACCELERATION_LENGTH = 0.8f;

	private: const float STICK_LOOK_DEADZONE = 0.19f;

	private: const float STICK_LOOK_HORIZ_MULTIPLIER = STICK_LOOK_VERT_FACTOR / STICK_LOOK_HORIZ_FACTOR;

	private: const float TURN_ACCELERATION_LENGTH = 0.8f;
	private: float TargetTurnFactor = 0.0f;
	private: float CurrentTurnFactor = 0.0f;


	/*--- Primary Player Rotation Function ---*/

	/** Calculates absolute 2D vector representing camera rotation from current player state given stick input.
	 *  Input: Normalized stick input (x=[0,1], y=[0,1])
	 *  Output: Absolute camera rotation for given frame (x=[0,infinity], y=[0,infinity])
	 */
	public: FVector2D calculatePlayerRotation(FVector2D Input, float TimeDelta);


	/*--- Public Functions ---*/

	// TODO - Add Functions


	/*--- Private Functions ---*/

	/** Calculates noralized 2D vector representing camera rotation from current player state given stick input.
	 *  Input: Normalized stick input [0,1]
	 *  Output: Normalized rotation factor [0,1]
	 */
	//private: FVector2D calculateNormalizedRotation(FVector2D Input);

	private: float CalculateTurnFactor(FVector2D Input, float TimeDelta);

	private: bool IsInTurnZone(FVector2D Input);

	private: float GetRadialFalloff(FVector2D Input);

};

