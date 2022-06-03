
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

	private: bool IsStickLeftPressed = false;

	private: bool IsFaceTopPressed = false;
	private: bool IsFaceLeftPressed = false;
	private: bool IsFaceRightPressed = false;
	private: bool IsFaceBottomPressed = false;

	private: bool IsBumperLeftPressed = false;

	private: bool IsDirectionalUpPressed = false;


	/*--- Action Binding Delegates ---*/

	private: StickLeftDelegate StickLeftEvent = nullptr;
	private: StickRightDelegate StickRightEvent = nullptr;

	private: StickLeftPressDelegate StickLeftPressEvent = nullptr;
	private: StickLeftReleaseDelegate StickLeftReleaseEvent = nullptr;
	private: StickRightPressDelegate StickRightPressEvent = nullptr;
	private: StickRightReleaseDelegate StickRightReleaseEvent = nullptr;

	private: FaceBottomPressDelegate FaceTopPressEvent = nullptr;
	private: FaceBottomReleaseDelegate FaceTopReleaseEvent = nullptr;
	private: FaceBottomPressDelegate FaceLeftPressEvent = nullptr;
	private: FaceBottomReleaseDelegate FaceLeftReleaseEvent = nullptr;
	private: FaceBottomPressDelegate FaceRightPressEvent = nullptr;
	private: FaceBottomReleaseDelegate FaceRightReleaseEvent = nullptr;
	private: FaceBottomPressDelegate FaceBottomPressEvent = nullptr;
	private: FaceBottomReleaseDelegate FaceBottomReleaseEvent = nullptr;

	private: BumperLeftPressDelegate BumperLeftPressEvent = nullptr;
	private: BumperLeftReleaseDelegate BumperLeftReleaseEvent = nullptr;

	private: DirectionalUpPressDelegate DirectionalUpPressEvent = nullptr;
	private: DirectionalUpReleaseDelegate DirectionalUpReleaseEvent = nullptr;


	/*--- Action Binding Functions ---*/

	public: void BindStickLeft(UObject * InUserObject, const FName & InFunctionName);
	public: void BindStickRight(UObject * InUserObject, const FName & InFunctionName);
	public: void BindStickLeftPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindStickLeftRelease(UObject * InUserObject, const FName & InFunctionName);
	public: void BindStickRightPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindStickRightRelease(UObject * InUserObject, const FName & InFunctionName);

	public: void BindFaceTopPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindFaceTopRelease(UObject * InUserObject, const FName & InFunctionName);
	public: void BindFaceLeftPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindFaceLeftRelease(UObject * InUserObject, const FName & InFunctionName);
	public: void BindFaceRightPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindFaceRightRelease(UObject * InUserObject, const FName & InFunctionName);
	public: void BindFaceBottomPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindFaceBottomRelease(UObject * InUserObject, const FName & InFunctionName);

	public: void BindBumperLeftPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindBumperLeftRelease(UObject * InUserObject, const FName & InFunctionName);

	public: void BindDirectionalUpPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindDirectionalUpRelease(UObject * InUserObject, const FName & InFunctionName);


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

