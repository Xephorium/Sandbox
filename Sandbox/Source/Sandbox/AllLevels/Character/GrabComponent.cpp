#include "GrabComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "GrabbableComponent.h"
#include "Engine/World.h"

/*
 *  GrabberComponent.cpp                               Chris Cruzen
 *  Sandbox                                              09.22.2022
 *
 *    GrabberComponent gives its actor the ability to grab and move
 *  other objects in the scene.
 */


/*--- Macros ---*/

#define OUT


/*--- Lifecycle Functions ---*/

UGrabComponent::UGrabComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateGrabRaycast();

	// If Object Grabbed, Update Location
	if (PhysicsHandleComponent && PhysicsHandleComponent->GrabbedComponent) {
		PhysicsHandleComponent->SetTargetLocation(HoldPoint);
	}
}

void UGrabComponent::BeginPlay() {
	Super::BeginPlay();

	InitializeMemberClasses();
}


/*--- Behavior Functions ---*/

void UGrabComponent::GrabObject() {
	AActor * RaycastHitActor = GetActorInView();

	// If Actor Found, Grab
	if (RaycastHitActor && !IsGrabbing) {
		ObjectPrimitiveComponent = Cast<UPrimitiveComponent>(
			RaycastHitActor->GetComponentByClass(UPrimitiveComponent::StaticClass())
		);
		IsGrabbing = true;

		// Grab Object
		PhysicsHandleComponent->GrabComponentAtLocation(
			ObjectPrimitiveComponent,
			NAME_None,
			RaycastHitActor->GetActorLocation()
		);

		// Reduce Angular Damping
		ObjectAngularDamping = ObjectPrimitiveComponent->GetAngularDamping();
		ObjectPrimitiveComponent->SetAngularDamping(HoldAngularDamping);
	}
}

void UGrabComponent::ReleaseObject() {
	if (IsGrabbing) {
		IsGrabbing = false;

		// Release Object
		PhysicsHandleComponent->ReleaseComponent();

		// Restore Angular Damping
		ObjectPrimitiveComponent->SetAngularDamping(ObjectAngularDamping);
	} 
}

void UGrabComponent::InitializeMemberClasses() {

	// Search Owning Actor for a Specific Component Type (Returns first found!)
	PhysicsHandleComponent = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	// Null Check PhysicsHandleComponent
	if (!PhysicsHandleComponent) {
		UE_LOG(
			LogTemp,
			Error,
			TEXT("No PhysicsHandleComponent found on Actor %s"),
			*GetOwner()->GetName()
		);
	}
}

void UGrabComponent::UpdateGrabRaycast() {

	// Update Player Location/Rotation
	PlayerLocation = FVector();
	FRotator PlayerRotation = FRotator();
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerLocation,
		OUT PlayerRotation
	);

	// Update Raycast End Point 
	RaycastEndPoint = PlayerLocation + PlayerRotation.Vector() * PlayerReach;
	HoldPoint = PlayerLocation + PlayerRotation.Vector() * HoldDistance;
}

AActor *UGrabComponent::GetActorInView() {

	// Initialize Raycast Variables
	FHitResult RaycastHit;
	FCollisionQueryParams RaycastParams = FCollisionQueryParams(
		FName(TEXT("")), // ???
		false, // Whether to use Visibility Collision
		GetOwner()// Any Actor to ignore? (Player!)
	);

	// Perform Raycast
	GetWorld()->LineTraceSingleByObjectType(
		OUT RaycastHit,
		PlayerLocation,
		RaycastEndPoint,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		RaycastParams
	);

	// Determine if Actor Returned
	if (RaycastHit.GetActor()) {

		// Determine if Actor Has GravityComponent
		UActorComponent *Component = RaycastHit.GetActor()->FindComponentByClass(UGrabbableComponent::StaticClass());
		if (Component) {

			// Determine if Grabbing Enabled
			UGrabbableComponent *GrabbableComponent = Cast<UGrabbableComponent>(Component);
			if (GrabbableComponent && GrabbableComponent->Grabbable) {

				// Return Actor
				return RaycastHit.GetActor();
			}
		}
	}

	// No Actor Found
	return nullptr;
}