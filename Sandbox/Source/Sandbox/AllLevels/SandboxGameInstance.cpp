
#include "SandboxGameInstance.h"
#include "Utility/LogUtility.h"

void USandboxGameInstance::StartGameInstance() {
    Super::StartGameInstance();

	// Test Function Call
	UE_LOG(LogTemp, Warning, TEXT("Llamas - Begin play!"));

	// Generate Steam ID, Initialize Client, Get User ID
	FFileHelper::SaveStringToFile(TEXT(RAW_APP_ID), TEXT("steam_appid.txt"));
	SteamAPI_RestartAppIfNecessary(atoi(APP_ID));
	if (SteamAPI_Init()) {
		SteamUserId = SteamUser()->GetSteamID();
	}
}

void USandboxGameInstance::PrintTestStringFromCode(FString text) {
	DebugLog(text);
}
