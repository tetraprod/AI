#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Token.h"
#include "HellHoundCharacter.h"
#include "Localization.h"
#include "ShoutToken.h"
#include "WizardPlayerState.generated.h"

USTRUCT(BlueprintType)
struct FWizardAppearance
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor SkinColor = FLinearColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor HairColor = FLinearColor::Black;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor EyeColor = FLinearColor(0.1f, 0.1f, 0.1f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Height = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString BodyType = TEXT("Average");

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HairStyle = TEXT("Short");

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RobeName = TEXT("TieDye");

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor RobeColor = FLinearColor::White;
};

/**
 * A record of a custom spell assembled from tokens.
 */
USTRUCT(BlueprintType)
struct FSpellLogEntry
{
    GENERATED_BODY();

    /** Player provided name for the spell */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SpellName;

    /** Tokens that make up the spell chain */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<UToken*> Tokens;
};

UCLASS()
class AWizardPlayerState : public APlayerState
{
    GENERATED_BODY()
public:
    AWizardPlayerState();
    /** Tokens owned by the player */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tokens")
    TArray<UToken*> TokenInventory;

    /** Tokens currently wagered in a bet match */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Tokens")
    TArray<UToken*> WagerTokens;

    /** Experience points earned from matches */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
    int32 Experience;

    /** Increment experience and handle level-ups */
    UFUNCTION(BlueprintCallable, Category="Stats")
    void AddExperience(int32 Amount);

    /** Compute current level based on experience (max 1000) */
    UFUNCTION(BlueprintCallable, Category="Stats")
    int32 GetLevel() const;

    /** True if the player can instantly kill opponents */
    UFUNCTION(BlueprintCallable, Category="Stats")
    bool HasOneHitKill() const;

    /** Damage resistance multiplier granted by current level */
    UFUNCTION(BlueprintCallable, Category="Stats")
    float GetDamageResistance() const;

    /** Returns true if the special tie dye robe is equipped */
    UFUNCTION(BlueprintCallable, Category="Appearance")
    bool IsTieDyeRobeEquipped() const;

    /** Visual appearance information */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance")
    FWizardAppearance Appearance;

    /** Purchased hound companions */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Companion")
    TArray<TSubclassOf<AHellHoundCharacter>> OwnedHounds;

    /** Robes the player has purchased */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance")
    TArray<FString> OwnedRobes;

    /** Currently equipped robe */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance")
    FString EquippedRobe;

    /** Attack bonus provided by the equipped robe */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance")
    float RobeAttackBonus;

    /** Shield bonus provided by the equipped robe */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance")
    float RobeShieldBonus;

    /** Selected hound to spawn each match */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Companion")
    TSubclassOf<AHellHoundCharacter> EquippedHound;

    /** Spells the player has assembled */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spells")
    TArray<FSpellLogEntry> SpellLog;

    /** Preferred language for UI text */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Localization")
    ELanguage PreferredLanguage;

    /** Simple list of achievements earned */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spells")
    TArray<FString> Achievements;

    /** Taunt tokens the player has purchased */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Taunt")
    TArray<UShoutToken*> OwnedShoutAttacks;

    /** Which shout attack is equipped */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Taunt")
    UShoutToken* EquippedShoutAttack;

    /** Custom taunt message provided by the player */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Taunt")
    FString CustomTauntMessage;

    UFUNCTION(BlueprintCallable, Category="Taunt")
    FString GetCensoredTaunt() const;

    UFUNCTION(BlueprintCallable, Category="Taunt")
    void SetCustomTaunt(const FString& Message);

    /**
     * Add a new spell to the log. Returns true if it was new and grants an
     * achievement.
     */
    UFUNCTION(BlueprintCallable, Category="Spells")
    bool AddSpellToLog(const FString& SpellName, const TArray<UToken*>& Tokens);
};
