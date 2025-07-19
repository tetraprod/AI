#include "HellHoundCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "WizardCharacter.h"

AHellHoundCharacter::AHellHoundCharacter()
{
    HoundTier = EHellHoundType::Minor;

    switch (HoundTier)
    {
    case EHellHoundType::Minor:
        AttackDamage = 10.f;
        MaxHealth = 50.f;
        break;
    case EHellHoundType::Greater:
        AttackDamage = 20.f;
        MaxHealth = 100.f;
        break;
    case EHellHoundType::Dire:
    default:
        AttackDamage = 30.f;
        MaxHealth = 150.f;
        break;
    }

    CurrentHealth = MaxHealth;
    OwnerWizard = nullptr;
    GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

float AHellHoundCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    CurrentHealth -= ActualDamage;
    if (CurrentHealth <= 0.f)
    {
        Die();
    }
    return ActualDamage;
}

void AHellHoundCharacter::Die()
{
    if (OwnerWizard)
    {
        OwnerWizard->OnCompanionKilled(this);
    }
    Destroy();
}
