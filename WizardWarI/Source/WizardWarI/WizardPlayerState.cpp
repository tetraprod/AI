#include "WizardPlayerState.h"

void AWizardPlayerState::AddExperience(int32 Amount)
{
    Experience += Amount;
    Experience = FMath::Max(0, Experience);
}

int32 AWizardPlayerState::GetLevel() const
{
    int32 Level = 1;
    int32 Threshold = 100;
    int32 Total = Threshold;
    while (Level < 1000 && Experience >= Total)
    {
        Level++;
        Threshold += (Level * 100);
        Total += Threshold;
    }
    return Level;
}

bool AWizardPlayerState::HasOneHitKill() const
{
    return GetLevel() >= 1000;
}

float AWizardPlayerState::GetDamageResistance() const
{
    int32 Level = GetLevel();
    return 1.f + (Level - 1) * 0.005f;
}
