#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "GrabbableComponent.h"

/*
 *  GrabbableComponent.cpp                              Chris Cruzen
 *  Sandbox                                               09.22.2022
 *
 *    GrabbableComponent implements grab behavior for the actors its
 *  attached to.
 */


/*--- Lifecycle Functions ---*/

UGrabbableComponent::UGrabbableComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabbableComponent::BeginPlay() {
	Super::BeginPlay();
}

