#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ElevatorComponent.generated.h"

/*
 *  ElevatorComponent.h                                 Chris Cruzen
 *  Sandbox                                               09.09.2022
 */


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SANDBOX_API UElevatorComponent : public UActorComponent {
	GENERATED_BODY()


/*--- Class Variables ---*/

public: UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=Gameplay) float CycleTime = 30.0f;

public: UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=Gameplay) float CycleHeight = 200.0f;

public: UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=Gameplay) bool CycleUpward = true;

private: float ElapsedTime = 0.0f;

private: FVector StartLocation = FVector();


/*--- Lifecycle Methods ---*/

public:	UElevatorComponent();

protected: virtual void BeginPlay() override;

public:	virtual void TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
) override;
		
};
