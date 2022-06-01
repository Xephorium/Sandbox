
#pragma once

#pragma warning(push)
#pragma warning(disable: 4996)
#include "Dependencies/Steam/Library/steam_api.h"
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


	/*--- Lifecycle Functions ---*/

	public: void OnTick(float DeltaTime);


	/*--- State Variables ---*/

	private: bool IsCrouchPressed = false;
	private: bool IsJumpPressed = false;
	private: bool IsRunPressed = false;
	private: bool IsFlyPressed = false;


	/*--- Action Binding Delegates ---*/

	private: MoveDelegate MoveEvent = nullptr;
	private: LookDelegate LookEvent = nullptr;
	private: CrouchPressDelegate CrouchPressEvent = nullptr;
	private: CrouchReleaseDelegate CrouchReleaseEvent = nullptr;
	private: JumpPressDelegate JumpPressEvent = nullptr;
	private: JumpReleaseDelegate JumpReleaseEvent = nullptr;
	private: RunPressDelegate RunPressEvent = nullptr;
	private: RunReleaseDelegate RunReleaseEvent = nullptr;
	private: FlyPressDelegate FlyPressEvent = nullptr;
	private: FlyReleaseDelegate FlyReleaseEvent = nullptr;


	/*--- Action Binding Functions ---*/

	public: void BindMove(UObject * InUserObject, const FName & InFunctionName);
	public: void BindLook(UObject * InUserObject, const FName & InFunctionName);
	public: void BindCrouchPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindCrouchRelease(UObject * InUserObject, const FName & InFunctionName);
	public: void BindJumpPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindJumpRelease(UObject * InUserObject, const FName & InFunctionName);
	public: void BindRunPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindRunRelease(UObject * InUserObject, const FName & InFunctionName);
	public: void BindFlyPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindFlyRelease(UObject * InUserObject, const FName & InFunctionName);


	/*--- Steam API ---*/

	/** Whether SteamInput is properly initialized */
	private: bool IsSteamInputInitialized;

	/** List of connected steam controllers **/
	private: InputHandle_t *controllers;

	/** Prepares SteamInput library for use - to be called from BeginPlay() **/
	public: void SetupSteamInput();

	/** Initializes SteamInput & records initialization state */
	private: void InitializeSteamInput();

	/** Checks SteamInput initialization state & whether class returns null */
	public: bool IsSteamInputAvailable();

	/** Utility Method - Gets Digital Action Data from SteamInput **/
	private: InputDigitalActionData_t ReadDigitalActionData(char* name);

	/** Utility Method - Gets Analog Action Data from SteamInput **/
	private: InputAnalogActionData_t ReadAnalogActionData(char* name);

};

