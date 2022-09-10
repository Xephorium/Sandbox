#include "FirstPersonLevel/CarouselComponent.h"

/*
 *  CarouselComponent.cpp                            Chris Cruzen
 *  Sandbox                                            09.10.2022
 *
 *    CarouselComponent runs a looping spin animation on whatever
 *  object the component is attached to.
 */


/*--- Lifecycle Functions ---*/

UCarouselComponent::UCarouselComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UCarouselComponent::BeginPlay() {
	Super::BeginPlay();
}

void UCarouselComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	ElapsedTime += DeltaTime;

	UPrimitiveComponent *OwnerPrimitiveComponent = Cast<UPrimitiveComponent>(
		GetOwner()->GetComponentByClass(UPrimitiveComponent::StaticClass())
	);

	OwnerPrimitiveComponent->SetWorldRotation(
		FRotator(
			0.0f,
			0.0f + ElapsedTime * SpinSpeed,
			0.0f
		)
	);
}

