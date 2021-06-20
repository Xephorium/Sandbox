
#include "LogUtility.h"
#include "Engine/GameEngine.h"

void DebugLog(FString text) {
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, text);
}
