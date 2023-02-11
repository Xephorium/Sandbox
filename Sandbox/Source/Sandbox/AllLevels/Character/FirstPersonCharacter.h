// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FirstPersonCharacter.generated.h"

class UInputComponent;
class UCameraComponent;
class UGamepadLookAdapter;
class UGrabComponent;
class USteamInputComponent;

/*
 *  FirstPersonCharacter.h                          Chris Cruzen
 *  Sandbox                                           05.29.2022
 *
 *  Header file for FirstPersonCharacter.cpp.
 */


UCLASS(Blueprintable, config=Game)
class AFirstPersonCharacter : public ACharacter {

	GENERATED_BODY()

	/*--- Constants  ---*/

	private: const float STICK_MOVE_DEADZONE = 0.18f;

	private: const float DEFAULT_CAPSULE_HEIGHT = 96.0f;
	private: const float DEFAULT_CAPSULE_RADIUS = 36.0f;
	private: const float DEFAULT_EYE_HEIGHT = 56.5f;

	private: const float VERTICAL_FLIGHT_SPEED = 1.0f;
	private: const float DEFAULT_JUMP_VELOCITY = 475.0f;
	private: const float DEFAULT_AIR_CONTROL = 0.2f;


	/*--- Variables ---*/

	/** Whether character is currently flying */
	public: UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	bool IsFlying;

	/** Whether character is capable of grabbing objects */
	public: UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	bool IsGrabEnabled = true;

	/** First person camera */
	protected: UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	protected: UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USteamInputComponent* SteamInputComponent;

	protected: UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UGamepadLookAdapter* GamepadLookAdapter;

	protected: UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UGrabComponent* GrabComponent;

	private: FVector2D CurrentMoveInput = FVector2D::ZeroVector;
	private: FVector2D CurrentLookInput = FVector2D::ZeroVector;

	private: float VerticalForceUp = 0.0f;
	private: float VerticalForceDown = 0.0f;


	/*--- Lifecycle Functions ---*/

	public: AFirstPersonCharacter();

	protected: virtual void BeginPlay() override;
	protected: virtual void Tick(float DeltaSeconds) override;


	/*--- Input Setup Functions ---*/

	/** APawn Override **/
	protected: virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	protected: void SetupSteamInputComponent();


	/*--- Input Handling Functions ---*/

	protected: UFUNCTION()
	void OnStickMove(FVector2D Input);

	protected: UFUNCTION()
	void OnStickLook(FVector2D Input);

	protected: UFUNCTION()
	void OnTriggerRight(float Input);

	protected: UFUNCTION()
	void OnTriggerLeft(float Input);

	protected: UFUNCTION()
	void OnStickLeftPress();

	protected: UFUNCTION()
	void OnStickRightPress();

	protected: UFUNCTION()
	void OnStartPress();

	protected: UFUNCTION()
	void OnEndPress();

	protected: UFUNCTION()
	void OnFaceTopPress();

	protected: UFUNCTION()
	void OnFaceLeftPress();

	protected: UFUNCTION()
	void OnFaceRightPress();

	protected: UFUNCTION()
	void OnFaceBottomPress();

	protected: UFUNCTION()
	void OnFaceBottomRelease();

	protected: UFUNCTION()
	void OnBumperLeftPress();

	protected: UFUNCTION()
	void OnBumperLeftRelease();

	protected: UFUNCTION()
	void OnBumperRightPress();

	protected: UFUNCTION()
	void OnBumperRightRelease();

	protected: UFUNCTION()
	void OnDPadUpPress();

	protected: UFUNCTION()
	void OnDPadLeftPress();
	
	protected: UFUNCTION()
	void OnDPadRightPress();

	protected: UFUNCTION()
	void OnDPadDownPress();


	/*--- Event Handling Functions ---*/

	protected: UFUNCTION()
	void OnControllerConnected();

	protected: UFUNCTION()
	void OnControllerDisconnected();


	/*--- Unreal Input State Functions ---*/

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
	 *       can call OnStickLook() with them on Tick(). Same for OnStickMove().
	 */

	protected: UFUNCTION()
	void OnStickMoveX(float Val);

	protected: UFUNCTION()
	void OnStickMoveY(float Val);

	protected: UFUNCTION()
	void OnStickLookX(float Input);

	protected: UFUNCTION()
	void OnStickLookY(float Input);

};
