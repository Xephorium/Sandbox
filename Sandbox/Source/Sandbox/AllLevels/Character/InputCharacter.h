#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputCharacter.generated.h"

class UInputComponent;
class USteamInputComponent;

/*
 *  InputCharacter.h                                      Chris Cruzen
 *  Sandbox                                                 02.11.2023
 *
 *  Header file for InputCharacter.cpp.
 */

UCLASS(Blueprintable, config=Game)
class AInputCharacter : public ACharacter {

	GENERATED_BODY()

	/*--- Constants  ---*/

	private: const float STICK_LEFT_DEADZONE = 0.18f;
	private: const float STICK_RIGHT_DEADZONE = 0.18f;


	/*--- Variables ---*/

	protected: UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USteamInputComponent* SteamInputComponent;

	private: FVector2D CurrentLeftStickInput = FVector2D::ZeroVector;
	private: FVector2D CurrentRightStickInput = FVector2D::ZeroVector;


	/*--- Lifecycle Functions ---*/

	public: AInputCharacter();

	protected: virtual void BeginPlay() override;
	protected: virtual void Tick(float DeltaSeconds) override;


	/*--- Setup Functions ---*/

	// APawn Override
	protected: virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	protected: void SetupSteamInputComponent();


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
	virtual void OnTriggerRight(float Input);

	protected: UFUNCTION()
	virtual void OnTriggerLeft(float Input);

	protected: UFUNCTION()
	virtual void OnStickLeftPress();

	protected: UFUNCTION()
	virtual void OnStickRightPress();

	protected: UFUNCTION()
	virtual void OnStartPress();

	protected: UFUNCTION()
	virtual void OnEndPress();

	protected: UFUNCTION()
	virtual void OnFaceTopPress();

	protected: UFUNCTION()
	virtual void OnFaceLeftPress();

	protected: UFUNCTION()
	virtual void OnFaceRightPress();

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
	virtual void OnDPadLeftPress();
	
	protected: UFUNCTION()
	virtual void OnDPadRightPress();

	protected: UFUNCTION()
	virtual void OnDPadDownPress();


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
	void OnStickLeftX(float Val);

	protected: UFUNCTION()
	void OnStickLeftY(float Val);

	protected: UFUNCTION()
	void OnStickRightX(float Input);

	protected: UFUNCTION()
	void OnStickRightY(float Input);

	/* Note: The below functions account for deadzone before delegating to the
	 *       final, overridable OnStickLeft() and OnStickRight() methods.
	 */
	
	protected: UFUNCTION()
	void OnStickLeftInput(FVector2D Input);

	protected: UFUNCTION()
	void OnStickRightInput(FVector2D Input);
};
