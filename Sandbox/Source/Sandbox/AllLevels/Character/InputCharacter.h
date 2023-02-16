#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputCharacter.generated.h"

class UControllerDiagnosticWidget;
class UInputComponent;
class USteamInputComponent;

/*
 *  InputCharacter.h                                      Chris Cruzen
 *  Sandbox                                                 02.11.2023
 *
 *  Header file for InputCharacter.cpp.
 */

UCLASS(Blueprintable, config=Game)
class SANDBOX_API AInputCharacter : public ACharacter {

	GENERATED_BODY()

	/*--- Constants  ---*/

	private: const float TOGGLE_CONTROLLER_DIAGNOSTIC_HOLD_TIME = 3.0f;

	private: const float STICK_LEFT_DEADZONE = 0.18f;
	private: const float STICK_RIGHT_DEADZONE = 0.18f;


	/*--- Variables ---*/

	protected: UPROPERTY()
	USteamInputComponent* SteamInputComponent;

	public: UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category="Input Debugging"))
	bool IsDebugLoggingEnabled = true;

	public: UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Category="Input Debugging"))
	bool IsControllerDiagnosticEnabled = true;

	public: UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (Category="Input Debugging"))
	TSubclassOf<UControllerDiagnosticWidget> ControllerDiagnosticWidgetClass;

	private: UControllerDiagnosticWidget* ControllerDiagnosticWidget;

	private: FVector2D CurrentUnrealStickLeftInput = FVector2D::ZeroVector;
	private: FVector2D CurrentUnrealStickRightInput = FVector2D::ZeroVector;

	private: bool IsHoldingToToggleControllerDiagnostic = false;
	private: float ToggleControllerDiagnosticCurrentTime = 0.0f;


	/*--- Lifecycle Functions ---*/

	public: AInputCharacter();

	protected: virtual void BeginPlay() override;
	protected: virtual void Tick(float DeltaSeconds) override;


	/*--- Setup Functions ---*/

	// APawn Override
	protected: virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	protected: void SetupSteamInputComponent();


	/*--- Diagnostic Functions ---*/

	private: void SetupControllerDiagnosticWidget();

	private: void ShowControllerDiagnosticWidget();

	private: void HideControllerDiagnosticWidget();


	/*--- Overridable Input Handling Functions ---*/

	protected: UFUNCTION()
	virtual void OnMouseHorizontal(float Input);

	protected: UFUNCTION()
	virtual void OnMouseVertical(float Input);

	protected: UFUNCTION()
	virtual void OnStickLeft(FVector2D Input);

	protected: UFUNCTION()
	virtual void OnStickRight(FVector2D Input);
	
	protected: UFUNCTION()
	virtual void OnTriggerLeft(float Input);

	protected: UFUNCTION()
	virtual void OnTriggerRight(float Input);

	protected: UFUNCTION()
	virtual void OnStickLeftPress();

	protected: UFUNCTION()
	virtual void OnStickLeftRelease();

	protected: UFUNCTION()
	virtual void OnStickRightPress();

	protected: UFUNCTION()
	virtual void OnStickRightRelease();

	protected: UFUNCTION()
	virtual void OnStartPress();

	protected: UFUNCTION()
	virtual void OnStartRelease();

	protected: UFUNCTION()
	virtual void OnEndPress();

	protected: UFUNCTION()
	virtual void OnEndRelease();

	protected: UFUNCTION()
	virtual void OnFaceTopPress();

	protected: UFUNCTION()
	virtual void OnFaceTopRelease();

	protected: UFUNCTION()
	virtual void OnFaceLeftPress();

	protected: UFUNCTION()
	virtual void OnFaceLeftRelease();

	protected: UFUNCTION()
	virtual void OnFaceRightPress();

	protected: UFUNCTION()
	virtual void OnFaceRightRelease();

	protected: UFUNCTION()
	virtual void OnFaceBottomPress();

	protected: UFUNCTION()
	virtual void OnFaceBottomRelease();

	protected: UFUNCTION()
	virtual void OnBumperLeftPress();

	protected: UFUNCTION()
	virtual void OnBumperLeftRelease();

	protected: UFUNCTION()
	virtual void OnBumperRightPress();

	protected: UFUNCTION()
	virtual void OnBumperRightRelease();

	protected: UFUNCTION()
	virtual void OnDPadUpPress();

	protected: UFUNCTION()
	virtual void OnDPadUpRelease();

	protected: UFUNCTION()
	virtual void OnDPadLeftPress();

	protected: UFUNCTION()
	virtual void OnDPadLeftRelease();
	
	protected: UFUNCTION()
	virtual void OnDPadRightPress();

	protected: UFUNCTION()
	virtual void OnDPadRightRelease();

	protected: UFUNCTION()
	virtual void OnDPadDownPress();

	protected: UFUNCTION()
	virtual void OnDPadDownRelease();


	/*--- Overridable Event Handling Functions ---*/

	protected: UFUNCTION()
	virtual void OnControllerConnected();

	protected: UFUNCTION()
	virtual void OnControllerDisconnected();


	/*--- Private Input Handling Functions ---*/

	/* Note: The below functions are a workaround to unreal's input system being
	 *       dumb. Ideally, we could bind OnStickLook(FVector Input) to both the
	 *       PlayerInputComponent and SteamInputComponent, letting that one
	 *       function manage all stick input regardless of origin. That said,
	 *       unreal's input system refuses to recognize inputs sent via the
	 *       Gamepad Right Thumbstick Axis-2D configuration. The FVector always
	 *       just comes back as (0.0, 0.0, 0.0). I have no clue why and am tired
	 *       of trying to diagnose something that would just work in any game
	 *       engine that wasn't build by baboons. For now, I've just setup two
	 *       functions, one for each axial input, to store the values so that I
	 *       can call OnStickLeft() with them on Tick(). Same for OnStickRight().
	 */

	protected: UFUNCTION()
	void OnUnrealStickLeftX(float Val);

	protected: UFUNCTION()
	void OnUnrealStickLeftY(float Val);

	protected: UFUNCTION()
	void OnUnrealStickRightX(float Input);

	protected: UFUNCTION()
	void OnUnrealStickRightY(float Input);


	/* Note: The below function normalizes unreal's very noisy, axis-specific
	 *       input to actually reflect a valid circular 2D input range. >_>
	 */

	protected: UFUNCTION()
	FVector2D NormalizeStickInput(FVector2D Input);


	/* Note: The below functions account for deadzone before delegating to the
	 *       final, overridable OnStickLeft() and OnStickRight() methods.
	 */
	
	protected: UFUNCTION()
	void OnStickLeftInput(FVector2D Input);

	protected: UFUNCTION()
	void OnStickRightInput(FVector2D Input);
};
