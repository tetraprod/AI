#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Token.h"
#include "WizardSaveGame.generated.h"

UCLASS()
class UWizardSaveGame : public USaveGame
{
    GENERATED_BODY()
public:
    UPROPERTY()
    int32 Experience;

    UPROPERTY()
    TArray<UToken*> TokenInventory;

    UPROPERTY()
    FLinearColor SkinColor;

    UPROPERTY()
    FLinearColor HairColor;

    UPROPERTY()
    float Height;
};
