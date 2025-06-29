#include "WizardWarIGameModeBase.h"

=======


=======
=======




#include "WizardPlayerState.h"
#include "WizardSaveGame.h"
#include "Kismet/GameplayStatics.h"


bool AWizardWarIGameModeBase::HostBetMatch(AWizardPlayerState* HostPlayer, const TArray<UToken*>& Tokens)
{
    if (PendingHost || !HostPlayer)
    {
        return false;
    }

    PendingHost = HostPlayer;
    PendingWager = Tokens;
    HostPlayer->WagerTokens = Tokens;
    return true;
}

bool AWizardWarIGameModeBase::JoinBetMatch(AWizardPlayerState* JoiningPlayer, const TArray<UToken*>& Tokens)
{
    if (!PendingHost || !JoiningPlayer)
    {
        return false;
    }

    if (Tokens.Num() != PendingWager.Num())
    {
        return false;
    }
    for (int32 i = 0; i < Tokens.Num(); ++i)
    {
        if (!Tokens[i] || !PendingWager[i] || Tokens[i]->TokenType != PendingWager[i]->TokenType)
        {
            return false;
        }
    }

    JoiningPlayer->WagerTokens = Tokens;
    // Multiplayer session setup would be handled here
    return true;
}

void AWizardWarIGameModeBase::ResolveBet(AWizardPlayerState* Winner, AWizardPlayerState* Loser)
{
    if (!Winner || !Loser)
    {
        return;
    }

    Winner->TokenInventory.Append(Winner->WagerTokens);
    Winner->TokenInventory.Append(Loser->WagerTokens);

    Winner->WagerTokens.Empty();
    Loser->WagerTokens.Empty();

    PendingHost = nullptr;
    PendingWager.Empty();
}


bool AWizardWarIGameModeBase::StartArenaBattle()
{
    if (bArenaActive)
    {
        return false;
    }

    bArenaActive = true;
    ArenaStartTime = GetWorld()->GetTimeSeconds();
    ArenaPlayers.Empty();
    ArenaPool.Empty();
    return true;
}

bool AWizardWarIGameModeBase::JoinArenaBattle(AWizardPlayerState* Player, const TArray<UToken*>& Tokens)
{
    if (!bArenaActive || !Player || ArenaPlayers.Num() >= 20)
    {
        return false;
    }

    ArenaPlayers.Add(Player);
    Player->WagerTokens = Tokens;
    ArenaPool.Append(Tokens);
    return true;
}

void AWizardWarIGameModeBase::EndArenaBattle(const TArray<AWizardPlayerState*>& Survivors)
{
    if (!bArenaActive)
    {
        return;
    }

    const bool bLasted = (GetWorld()->GetTimeSeconds() - ArenaStartTime) >= 60.f;

    for (AWizardPlayerState* Player : ArenaPlayers)
    {
        if (!Player)
        {
            continue;
        }

        if (Survivors.Contains(Player) && bLasted)
        {
            TArray<UToken*> Reward = Player->WagerTokens;
            Reward.Append(Player->WagerTokens);
            Player->TokenInventory.Append(Reward);
        }
        else
        {
            ArenaPool.Append(Player->WagerTokens);
        }

        Player->WagerTokens.Empty();
    }

    ArenaPlayers.Empty();
    bArenaActive = false;
}

void AWizardWarIGameModeBase::ResolveDailyDeathmatch(AWizardPlayerState* Winner)
{
    if (!Winner || ArenaPool.Num() == 0)
    {
        return;
    }

    Winner->TokenInventory.Append(ArenaPool);
    ArenaPool.Empty();
}

=======

void AWizardWarIGameModeBase::AwardMatchXP(AWizardPlayerState* Player, float MatchLengthSeconds)
{
    if (!Player)
    {
        return;
    }

    int32 XP = FMath::RoundToInt(MatchLengthSeconds);
    if (MatchLengthSeconds > 30.f)
    {
        XP *= 2;
    }
    if (MatchLengthSeconds > 90.f)
    {
        XP *= 2;
    }

    Player->AddExperience(XP);
}

bool AWizardWarIGameModeBase::SavePlayer(AWizardPlayerState* Player, const FString& SlotName)
{
    if (!Player)
    {
        return false;
    }

    UWizardSaveGame* SaveGame = Cast<UWizardSaveGame>(UGameplayStatics::CreateSaveGameObject(UWizardSaveGame::StaticClass()));
    if (!SaveGame)
    {
        return false;
    }

    SaveGame->Experience = Player->Experience;
    SaveGame->TokenInventory = Player->TokenInventory;
    SaveGame->SkinColor = Player->SkinColor;
    SaveGame->HairColor = Player->HairColor;
    SaveGame->Height = Player->Height;

    return UGameplayStatics::SaveGameToSlot(SaveGame, SlotName, 0);
}

bool AWizardWarIGameModeBase::LoadPlayer(AWizardPlayerState* Player, const FString& SlotName)
{
    if (!Player)
    {
        return false;
    }

    if (USaveGame* Loaded = UGameplayStatics::LoadGameFromSlot(SlotName, 0))
    {
        UWizardSaveGame* SaveGame = Cast<UWizardSaveGame>(Loaded);
        if (!SaveGame)
        {
            return false;
        }

        Player->Experience = SaveGame->Experience;
        Player->TokenInventory = SaveGame->TokenInventory;
        Player->SkinColor = SaveGame->SkinColor;
        Player->HairColor = SaveGame->HairColor;
        Player->Height = SaveGame->Height;
        return true;
    }

    return false;
}


=======
=======
=======



void AWizardWarIGameModeBase::StartPlay()
{
    Super::StartPlay();
    if (MenuMusic)
    {
        UGameplayStatics::SpawnSound2D(this, MenuMusic);
    }
}

=======
=======
=======




