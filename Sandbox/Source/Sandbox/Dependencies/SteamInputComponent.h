
#pragma once

#include "CoreMinimal.h"
#pragma warning(push)
#pragma warning(disable: 4996)
#include "Dependencies/Steam/steam_api.h"
#pragma warning(pop)
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


	/*--- Lifecycle Methods ---*/

	public: void OnTick(float DeltaTime);


	/*--- Action Binding Functions ---*/

	public: void BindJumpPress(UObject * InUserObject, const FName & InFunctionName);


	/*--- Steam API ---*/

	/** Whether SteamInput is properly initialized */
	private: bool IsSteamInputInitialized;

	/** List of connected steam controllers **/
	private: InputHandle_t *ConnectedSteamControllers;

	/** ... */
	public: void SetupSteamInput();

	/** Initializes SteamInput & records initialization state */
	private: void InitializeSteamInput();

	/** Checks SteamInput initialization state & whether class returns null */
	public: bool IsSteamInputAvailable();


	/*--- Action Binding Delegates ---*/

	private: JumpPressDelegate JumpPressEvent = nullptr;
	private: JumpReleaseDelegate JumpReleaseEvent = nullptr;

};

