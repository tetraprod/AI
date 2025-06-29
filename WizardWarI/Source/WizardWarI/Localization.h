#pragma once
#include "CoreMinimal.h"
#include "Localization.generated.h"

UENUM(BlueprintType)
enum class ELanguage : uint8
{
    English     UMETA(DisplayName="English"),
    Spanish     UMETA(DisplayName="Spanish"),
    French      UMETA(DisplayName="French"),
    German      UMETA(DisplayName="German")
};
