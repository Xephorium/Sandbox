#pragma once

#include "CoreMinimal.h"
#include "InputCharacter.h"
#include "FirstPersonCharacter.generated.h"

class UCameraComponent;
class UCapsuleComponent;
class CharacterMovementComponent;
class UControllerDiagnosticWidget;
class UGamepadLookAdapter;
class UGrabComponent;

/*
 *  FirstPersonCharacter.h                          Chris Cruzen
 *  Sandbox                                           05.29.2022
 *
 *  Header file for FirstPersonCharacter.cpp.
 */


UCLASS(Blueprintable, config=Game)
class AFirstPersonCharacter : public AInputCharacter {

	GENERATED_BODY()

	/*--- Constants  ---*/

	private: const float DEFAULT_CAPSULE_HEIGHT = 96.0f;
	private: const float DEFAULT_CAPSULE_RADIUS = 36.0f;
	private: const float DEFAULT_EYE_HEIGHT = 56.5f;

	private: const float DEFAULT_JUMP_VELOCITY = 475.0f;
	private: const float DEFAULT_AIR_CONTROL = 0.2f;

	private: const float HOME_CAPSULE_RADIUS = 18.0f;

	private: const float FLIGHT_VERTICAL_SPEED = 0.8f;


	/*--- Variables ---*/

	public: UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	bool IsFlyingEnabled = true;

	public: UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Gameplay)
	bool IsFlying;

	public: UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	bool IsGrabEnabled = true;

	protected: UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	protected: UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UGamepadLookAdapter* GamepadLookAdapter;

	protected: UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UGrabComponent* GrabComponent;

	private: float VerticalForceUp = 0.0f;
	private: float VerticalForceDown = 0.0f;


	/*--- Lifecycle Functions ---*/

	public: AFirstPersonCharacter();

	protected: virtual void BeginPlay() override;
	protected: virtual void Tick(float DeltaSeconds) override;


	/*--- First Person Setup Functions ---*/

	protected: void SetupGamepadLookAdapter();

	protected: void SetupGrabComponent();


	/*--- Input Handling Overrides ---*/

	virtual void OnMouseHorizontal(float Input) override;

	virtual void OnMouseVertical(float Input) override;

	virtual void OnStickLeft(FVector2D Input) override;

	virtual void OnStickRight(FVector2D Input) override;

	virtual void OnFaceBottomPress() override;

	virtual void OnFaceBottomRelease() override;

	virtual void OnFaceRightPress() override;

	virtual void OnBumperLeftPress() override;

	virtual void OnBumperLeftRelease() override;

	virtual void OnBumperRightPress() override;

	virtual void OnBumperRightRelease() override;

	virtual void OnDPadUpPress() override;

};
