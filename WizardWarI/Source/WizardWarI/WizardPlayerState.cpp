#include "WizardPlayerState.h"
#include "Localization.h"

AWizardPlayerState::AWizardPlayerState()
{
    Experience = 0;
    EquippedHound = nullptr;
    OwnedRobes.Add(TEXT("TieDye"));
    EquippedRobe = TEXT("TieDye");
    Appearance.RobeName = TEXT("TieDye");
    RobeAttackBonus = 0.f;
    RobeShieldBonus = 0.f;
    PreferredLanguage = ELanguage::English;
    EquippedShoutAttack = nullptr;
    CustomTauntMessage = TEXT("For glory!");
}
=======

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

bool AWizardPlayerState::IsTieDyeRobeEquipped() const
{
    return EquippedRobe.Equals(TEXT("TieDye"));
}

bool AWizardPlayerState::AddSpellToLog(const FString& SpellName, const TArray<UToken*>& Tokens)
{
    // Check for an existing identical spell
    for (const FSpellLogEntry& Entry : SpellLog)
    {
        if (Entry.Tokens.Num() == Tokens.Num())
        {
            bool bMatch = true;
            for (int32 i = 0; i < Tokens.Num(); ++i)
            {
                if (!Tokens[i] || !Entry.Tokens[i] || Tokens[i]->TokenType != Entry.Tokens[i]->TokenType)
                {
                    bMatch = false;
                    break;
                }
            }
            if (bMatch)
            {
                return false; // already known
            }
        }
    }

    FSpellLogEntry NewEntry;
    NewEntry.SpellName = SpellName;
    NewEntry.Tokens = Tokens;
    SpellLog.Add(NewEntry);

    Achievements.Add(FString::Printf(TEXT("Discovered %s"), *SpellName));
    return true;
}

FString AWizardPlayerState::GetCensoredTaunt() const
{
    FString Text = CustomTauntMessage;
    const TArray<FString> BadWords = { TEXT("damn"), TEXT("hell"), TEXT("shit"), TEXT("fuck") };
    for (const FString& Word : BadWords)
    {
        Text = Text.Replace(*Word, TEXT("****"), ESearchCase::IgnoreCase);
    }
    if (EquippedShoutAttack && !EquippedShoutAttack->DefaultMessage.IsEmpty())
    {
        Text = EquippedShoutAttack->DefaultMessage + TEXT(" ") + Text;
    }
    return Text;
}

void AWizardPlayerState::SetCustomTaunt(const FString& Message)
{
    CustomTauntMessage = Message;
}
=======
