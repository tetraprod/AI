#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Token.h"
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
};

UCLASS()
class AWizardPlayerState : public APlayerState
{
    GENERATED_BODY()
public:
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

    /** Visual appearance information */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance")
    FWizardAppearance Appearance;
};
