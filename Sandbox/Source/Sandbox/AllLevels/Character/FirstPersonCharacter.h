// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FirstPersonCharacter.generated.h"

class UInputComponent;
class UCameraComponent;
class UGamepadLookAdapter;
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

	/*--- Variables  ---*/

	private: const float STICK_MOVE_DEADZONE = 0.18f;

	private: FVector2D CurrentMoveInput = FVector2D::ZeroVector;
	private: FVector2D CurrentLookInput = FVector2D::ZeroVector;

	/** First person camera */
	protected: UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	protected: UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USteamInputComponent* SteamInputComponent;

	protected: UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UGamepadLookAdapter* GamepadLookAdapter;

	/** Whether character is currently flying */
	public: UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	bool IsFlying;


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
	void OnCrouchPress();

	protected: UFUNCTION()
	void OnCrouchRelease();

	protected: UFUNCTION()
	void OnJumpPress();

	protected: UFUNCTION()
	void OnJumpRelease();

	protected: UFUNCTION()
	void OnRunPress();

	protected: UFUNCTION()
	void OnRunRelease();

	protected: UFUNCTION()
	void OnFlyPress();

	protected: UFUNCTION()
	void OnFlyRelease();


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
