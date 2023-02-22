#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "GamepadType.generated.h"

UENUM(BlueprintType)
enum class EGamepadType : uint8 {
    GenericUnreal       UMETA(DisplayName = "Generic Unreal"),
	GenericSteam        UMETA(DisplayName = "Generic Steam"),
    Steam               UMETA(DisplayName = "Steam"),
    SteamDeck           UMETA(DisplayName = "Steam Deck"),
    Xbox360             UMETA(DisplayName = "Xbox 360"),
    XboxOne             UMETA(DisplayName = "Xbox One"),
    PlayStation3        UMETA(DisplayName = "PlayStation 3"),
    PlayStation4        UMETA(DisplayName = "PlayStation 4"),
    PlayStation5        UMETA(DisplayName = "PlayStation 5"),
    SwitchPro           UMETA(DisplayName = "Switch Pro"),
    SwitchJoyConSingle  UMETA(DisplayName = "Switch JoyCon Single"),
    SwitchJoyConPair    UMETA(DisplayName = "Switch JoyCon Pair"),
    Disconnected        UMETA(DisplayName = "Disconnected")
};