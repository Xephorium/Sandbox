#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "Components/PrimitiveComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "GrabComponent.generated.h"

/*
 *  GrabberComponent.cpp                               Chris Cruzen
 *  Sandbox                                              09.22.2022
 */

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UGrabComponent : public UActorComponent {
	GENERATED_BODY()


	/*--- Variables ---*/

	public: UPROPERTY(EditAnywhere) float PlayerReach = 300.f;
	public: UPROPERTY(EditAnywhere) float HoldDistance = 170.f;
	public: UPROPERTY(EditAnywhere) float HoldAngularDamping = 7.0f;
	public: bool IsGrabbing = false;

	private: UPhysicsHandleComponent *PhysicsHandleComponent = nullptr;
	private: UInputComponent *InputComponent = nullptr;
	private: FVector PlayerLocation;
	private: FVector RaycastEndPoint;
	private: FVector HoldPoint;

	private: UPrimitiveComponent * ObjectPrimitiveComponent;
	private: float ObjectAngularDamping;


	/*--- Lifecycle Functions ---*/

	public:	UGrabComponent();

	public:	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

	protected: virtual void BeginPlay() override;


	/*--- Behavior Functions ---*/

	public: void GrabObject();
	public: void ReleaseObject();
			
	private: void InitializeMemberClasses();
	private: void UpdateGrabRaycast();
	private: AActor *GetActorInView();

};
