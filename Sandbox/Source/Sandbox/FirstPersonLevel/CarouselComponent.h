#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CarouselComponent.generated.h"

/*
 *  CarouselComponent.h                              Chris Cruzen
 *  Sandbox                                            09.10.2022
 */


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SANDBOX_API UCarouselComponent : public UActorComponent {
	GENERATED_BODY()

/*--- Variables ---*/

public: UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=Gameplay) float SpinSpeed = 7.0f;

private: float ElapsedTime = 0.0f;


/*--- Lifecycle Functions ---*/

public:	UCarouselComponent();

protected: virtual void BeginPlay() override;

public:	virtual void TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction
) override;

};
