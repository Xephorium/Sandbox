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

	/** First person camera */
	protected: UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	protected: UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USteamInputComponent* SteamInputComponent;

	protected: UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UGamepadLookAdapter* GamepadLookAdapter;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	protected: UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	protected: UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

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
	void OnMoveForward(float Val);

	protected: UFUNCTION()
	void OnMoveRight(float Val);

	protected: UFUNCTION()
	void OnLook(FVector2D Input);

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

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	protected: void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	protected: void LookUpAtRate(float Rate);

};