
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
	private: bool IsStickRightPressed = false;

	private: bool IsStartPressed = false;
	private: bool IsEndPressed = false;

	private: bool IsFaceTopPressed = false;
	private: bool IsFaceLeftPressed = false;
	private: bool IsFaceRightPressed = false;
	private: bool IsFaceBottomPressed = false;

	private: bool IsBumperLeftPressed = false;
	private: bool IsBumperRightPressed = false;

	private: bool IsDPadUpPressed = false;
	private: bool IsDPadLeftPressed = false;
	private: bool IsDPadRightPressed = false;
	private: bool IsDPadDownPressed = false;


	/*--- Action Binding Delegates ---*/

	private: VectorInputDelegate StickLeftDelegate = nullptr;
	private: VectorInputDelegate StickRightDelegate = nullptr;
	
	private: FloatInputDelegate TriggerLeftDelegate = nullptr;
	private: FloatInputDelegate TriggerRightDelegate = nullptr;

	private: InputDelegate StickLeftPressDelegate = nullptr;
	private: InputDelegate StickLeftReleaseDelegate = nullptr;
	private: InputDelegate StickRightPressDelegate = nullptr;
	private: InputDelegate StickRightReleaseDelegate = nullptr;

	private: InputDelegate StartPressDelegate = nullptr;
	private: InputDelegate StartReleaseDelegate = nullptr;
	private: InputDelegate EndPressDelegate = nullptr;
	private: InputDelegate EndReleaseDelegate = nullptr;

	private: InputDelegate FaceTopPressDelegate = nullptr;
	private: InputDelegate FaceTopReleaseDelegate = nullptr;
	private: InputDelegate FaceLeftPressDelegate = nullptr;
	private: InputDelegate FaceLeftReleaseDelegate = nullptr;
	private: InputDelegate FaceRightPressDelegate = nullptr;
	private: InputDelegate FaceRightReleaseDelegate = nullptr;
	private: InputDelegate FaceBottomPressDelegate = nullptr;
	private: InputDelegate FaceBottomReleaseDelegate = nullptr;

	private: InputDelegate DPadUpPressDelegate = nullptr;
	private: InputDelegate DPadUpReleaseDelegate = nullptr;
	private: InputDelegate DPadLeftPressDelegate = nullptr;
	private: InputDelegate DPadLeftReleaseDelegate = nullptr;
	private: InputDelegate DPadRightPressDelegate = nullptr;
	private: InputDelegate DPadRightReleaseDelegate = nullptr;
	private: InputDelegate DPadDownPressDelegate = nullptr;
	private: InputDelegate DPadDownReleaseDelegate = nullptr;

	private: InputDelegate BumperLeftPressDelegate = nullptr;
	private: InputDelegate BumperLeftReleaseDelegate = nullptr;
	private: InputDelegate BumperRightPressDelegate = nullptr;
	private: InputDelegate BumperRightReleaseDelegate = nullptr;


	/*--- Action Binding Functions ---*/

	public: void BindStickLeft(UObject * InUserObject, const FName & InFunctionName);
	public: void BindStickRight(UObject * InUserObject, const FName & InFunctionName);

	public: void BindTriggerLeft(UObject * InUserObject, const FName & InFunctionName);
	public: void BindTriggerRight(UObject * InUserObject, const FName & InFunctionName);

	public: void BindStickLeftPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindStickLeftRelease(UObject * InUserObject, const FName & InFunctionName);
	public: void BindStickRightPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindStickRightRelease(UObject * InUserObject, const FName & InFunctionName);

	public: void BindStartPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindStartRelease(UObject * InUserObject, const FName & InFunctionName);
	public: void BindEndPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindEndRelease(UObject * InUserObject, const FName & InFunctionName);

	public: void BindFaceTopPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindFaceTopRelease(UObject * InUserObject, const FName & InFunctionName);
	public: void BindFaceLeftPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindFaceLeftRelease(UObject * InUserObject, const FName & InFunctionName);
	public: void BindFaceRightPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindFaceRightRelease(UObject * InUserObject, const FName & InFunctionName);
	public: void BindFaceBottomPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindFaceBottomRelease(UObject * InUserObject, const FName & InFunctionName);

	public: void BindDPadUpPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindDPadUpRelease(UObject * InUserObject, const FName & InFunctionName);
	public: void BindDPadLeftPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindDPadLeftRelease(UObject * InUserObject, const FName & InFunctionName);
	public: void BindDPadRightPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindDPadRightRelease(UObject * InUserObject, const FName & InFunctionName);
	public: void BindDPadDownPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindDPadDownRelease(UObject * InUserObject, const FName & InFunctionName);

	public: void BindBumperLeftPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindBumperLeftRelease(UObject * InUserObject, const FName & InFunctionName);
	public: void BindBumperRightPress(UObject * InUserObject, const FName & InFunctionName);
	public: void BindBumperRightRelease(UObject * InUserObject, const FName & InFunctionName);


	/*--- Steam API ---*/

	/** Whether SteamInput is properly initialized */
	private: bool IsSteamInputInitialized;

	/** List of connected steam controllers **/
	private: InputHandle_t *controllers;

	/** Sandbox Action Set Handle **/
	private: InputActionSetHandle_t SandboxSetHandle;

	/** Prepares SteamInput library for use - to be called from BeginPlay() **/
	public: void SetupSteamInput();

	/** Initializes SteamInput & records initialization state */
	private: void InitializeSteamInput();

	/** Checks SteamInput initialization state & whether class returns null */
	public: bool IsSteamInputAvailable();

	/** Checks for Connected Controllers **/
	private: void CheckForConnectedControllers();

	/** Utility Method - Gets Digital Action Data from SteamInput **/
	private: InputDigitalActionData_t GetDigitalInput(char* name);

	/** Utility Method - Gets Analog Action Data from SteamInput **/
	private: InputAnalogActionData_t GetAnalogInput(char* name);


	/*--- Input Delegation Functions ---*/

	private: void DelegateButtonInput(
		InputDigitalActionData_t Action,
		bool &IsPressed,
		InputDelegate DelegatePress,
		InputDelegate DelegateRelease
	);

	private: void DelegateTriggerInput(InputAnalogActionData_t Action, FloatInputDelegate Delegate);

	private: void DelegateStickInput(InputAnalogActionData_t Action, VectorInputDelegate Delegate);

	private: void DelegateYInvertedStickInput(InputAnalogActionData_t Action, VectorInputDelegate Delegate);

};

