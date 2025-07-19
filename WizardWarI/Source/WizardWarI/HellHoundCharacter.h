#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CompanionToken.h"
#include "HellHoundCharacter.generated.h"

UCLASS()
class AHellHoundCharacter : public ACharacter
{
    GENERATED_BODY()
public:
    AHellHoundCharacter();

    /** Damage inflicted per attack */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
    float AttackDamage;

    /** Maximum health based on hound tier */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
    float MaxHealth;

    /** Current hit points */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Combat")
    float CurrentHealth;

    /** Strength tier for scaling stats */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
    EHellHoundType HoundTier;

    /** Owning wizard for respawn callbacks */
    UPROPERTY()
    class AWizardCharacter* OwnerWizard;

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
        AController* EventInstigator, AActor* DamageCauser) override;

    void Die();
};
