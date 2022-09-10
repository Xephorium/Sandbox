
#include "FirstPersonLevel/ElevatorComponent.h"
#include "GenericPlatform/GenericPlatformMath.h"

/*
 *  ElevatorComponent.cpp                                 Chris Cruzen
 *  Sandbox                                                 09.09.2022
 *
 *    ElevatorComponent runs a looping rise/fall animation on whatever
 *  object the component is attached to.
 */


/*--- Lifecycle Functions ---*/

UElevatorComponent::UElevatorComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UElevatorComponent::BeginPlay() {
	Super::BeginPlay();
}

void UElevatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	ElapsedTime += DeltaTime;
	float Progress = FGenericPlatformMath::Fmod(ElapsedTime, CycleTime * 1000.0f) / CycleTime;
	if (!CycleUpward) { Progress = 1.0f - Progress; }

	UPrimitiveComponent *OwnerPrimitiveComponent = Cast<UPrimitiveComponent>(
		GetOwner()->GetComponentByClass(UPrimitiveComponent::StaticClass())
	);

	if (StartLocation == FVector()) {
		StartLocation = OwnerPrimitiveComponent->GetComponentLocation();
	}

	OwnerPrimitiveComponent->SetWorldLocation(
		FVector(
			StartLocation.X,
			StartLocation.Y,
			StartLocation.Z + (FMath::Sin((20.0f * Progress)/ 3.14159265f) / 2.0f + 0.5f) * CycleHeight
		)
	);
}

