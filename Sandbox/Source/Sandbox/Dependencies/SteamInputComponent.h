
#pragma once

#include "CoreMinimal.h"
#include "SteamInputComponentDelegates.h"
#include "SteamInputComponent.generated.h"

/*
 *  SteamInputComponent.h                          Chris Cruzen
 *  Sandbox                                           05.29.2022
 *
 *  Header file for SteamInputComponent.cpp.
 */

UCLASS()
class USteamInputComponent : public UObject {
	
	GENERATED_BODY()

public:

	USteamInputComponent();

	/*--- Action Binding Functions ---*/

	void BindJumpPress(UObject * InUserObject, const FName & InFunctionName);

private:

	/*--- Action Binding Delegates ---*/

	JumpPressDelegate JumpPressEvent = nullptr;
	JumpReleaseDelegate JumpReleaseEvent = nullptr;

};

