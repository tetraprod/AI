#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Token.h"
#include "WizardPlayerState.h"
#include "ShoutToken.h"
#include "Localization.h"
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

    UPROPERTY()
    FLinearColor EyeColor;

    UPROPERTY()
    FString BodyType;

    UPROPERTY()
    FString HairStyle;

    UPROPERTY()
    FString RobeName;

    UPROPERTY()
    FLinearColor RobeColor;

    UPROPERTY()
    TArray<FSpellLogEntry> SpellLog;

    UPROPERTY()
    TArray<FString> Achievements;

    UPROPERTY()
    TArray<FString> OwnedRobes;

    UPROPERTY()
    FString EquippedRobe;

    UPROPERTY()
    TArray<TSubclassOf<AHellHoundCharacter>> OwnedHounds;

    UPROPERTY()
    TSubclassOf<AHellHoundCharacter> EquippedHound;

    UPROPERTY()
    TArray<UShoutToken*> OwnedShoutAttacks;

    UPROPERTY()
    UShoutToken* EquippedShoutAttack;

    UPROPERTY()
    FString CustomTauntMessage;

    UPROPERTY()
    ELanguage PreferredLanguage;
};
