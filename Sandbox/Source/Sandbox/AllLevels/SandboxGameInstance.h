#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
#pragma warning(push)                              // Push current precompiler warning configuration to stack
#pragma warning(disable:4996)                      // Disable pesky steam api warnings
#include "Dependencies/Steam/Library/steam_api.h"  // Include steam api
#pragma warning(pop)                               // Restore previous precompiler warning configuration
#include "SandboxGameInstance.generated.h"

#define RAW_APP_ID "480"

UCLASS(Blueprintable, Category=AllLevels)
class SANDBOX_API USandboxGameInstance : public UAdvancedFriendsGameInstance {

	GENERATED_BODY()

protected:
	virtual void StartGameInstance() override;

public:
	UFUNCTION(BlueprintCallable)
	void PrintTestStringFromCode(FString text);

	CSteamID SteamUserId;

	static constexpr char* APP_ID = RAW_APP_ID;
};
