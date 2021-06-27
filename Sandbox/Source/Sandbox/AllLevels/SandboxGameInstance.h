#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
#include "SandboxGameInstance.generated.h"


UCLASS(Blueprintable, Category=AllLevels)
class SANDBOX_API USandboxGameInstance : public UAdvancedFriendsGameInstance {

	GENERATED_BODY()

	public:
		UFUNCTION(BlueprintCallable)
		void PrintTestStringFromCode(FString text);

};
