#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrabbableComponent.generated.h"

/*
 *  GrabbableComponent.h                                Chris Cruzen
 *  Sandbox                                               09.22.2022
 */

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UGrabbableComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	bool Grabbable = true;

public:
	UGrabbableComponent();

protected:
	virtual void BeginPlay() override;
		
};
