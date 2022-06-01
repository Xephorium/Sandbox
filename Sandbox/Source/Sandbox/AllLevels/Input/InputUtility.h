
#pragma once

#include "InputUtility.generated.h"

/*
 *  GamepadLookAdapter.h                            Chris Cruzen
 *  Sandbox                                           05.31.2022
 *
 *  Header file for GamepadLookAdapter.cpp.
 */

UCLASS()
class UInputUtility : public UObject {
	
	GENERATED_BODY()

	/* */
	public: static FVector2D AccommodateDeadzone(FVector2D Input, float Deadzone);
	                         

};

