#pragma once

#include "CoreMinimal.h"
#include "InputCharacter.h"
#include "FirstPersonCharacter.generated.h"

class UCameraComponent;
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
	UGamepadLookAdapter* GamepadLookAdapter;

	protected: UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UGrabComponent* GrabComponent;

	protected: UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UControllerDiagnosticWidget> ControllerDiagnosticWidget;

	private: float VerticalForceUp = 0.0f;
	private: float VerticalForceDown = 0.0f;


	/*--- Lifecycle Functions ---*/

	public: AFirstPersonCharacter();

	protected: virtual void BeginPlay() override;
	protected: virtual void Tick(float DeltaSeconds) override;


	/*--- Input Setup Functions ---*/

	protected: void SetupGamepadLookAdapter();

	protected: void SetupControllerDiagnosticWidget();

	protected: void SetupGrabComponent();


	/*--- Input Handling Overrides ---*/

	virtual void OnMouseHorizontal(float Input) override;

	virtual void OnMouseVertical(float Input) override;

	virtual void OnStickLeft(FVector2D Input) override;

	virtual void OnStickRight(FVector2D Input) override;

	virtual void OnFaceBottomPress() override;

	virtual void OnFaceBottomRelease() override;

	virtual void OnBumperLeftPress() override;

	virtual void OnBumperLeftRelease() override;

	virtual void OnBumperRightPress() override;

	virtual void OnBumperRightRelease() override;

};
