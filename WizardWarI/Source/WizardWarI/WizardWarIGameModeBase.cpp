#include "WizardWarIGameModeBase.h"
#include "WizardPlayerState.h"
#include "WizardSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "CastleHub.h"

AWizardWarIGameModeBase::AWizardWarIGameModeBase()
{
    FRobeData TieDye;
    TieDye.Name = TEXT("TieDye");
    TieDye.TokenCost = 0;
    AvailableRobes.Add(TieDye);

    FRobeData Speedy;
    Speedy.Name = TEXT("Speedy");
    Speedy.TokenCost = 50;
    Speedy.bSpeedy = true;
    Speedy.AttackBonus = 0.f;
    Speedy.ShieldBonus = 0.f;
    AvailableRobes.Add(Speedy);

    UShoutToken* Basic = NewObject<UShoutToken>();
    Basic->DefaultMessage = TEXT("Hyaa!");
    Basic->TokenCost = 5;
    AvailableShouts.Add(Basic);

    FEnvironmentEntry Dungeon;
    Dungeon.Type = ECombatEnvironment::Dungeon;
    AvailableEnvironments.Add(Dungeon);

    FEnvironmentEntry Forest;
    Forest.Type = ECombatEnvironment::Forest;
    AvailableEnvironments.Add(Forest);

    FEnvironmentEntry Island;
    Island.Type = ECombatEnvironment::ShrinkingIsland;
    AvailableEnvironments.Add(Island);

    FEnvironmentEntry Colosseum;
    Colosseum.Type = ECombatEnvironment::Colosseum;
    AvailableEnvironments.Add(Colosseum);

    FEnvironmentEntry Mountain;
    Mountain.Type = ECombatEnvironment::MountainTop;
    AvailableEnvironments.Add(Mountain);
}


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

bool AWizardWarIGameModeBase::BuyHellHound(AWizardPlayerState* Player, UCompanionToken* Token)
{
    if (!Player || !Token)
    {
        return false;
    }

    if (Player->TokenInventory.Num() < Token->TokenCost)
    {
        return false;
    }

    Player->TokenInventory.RemoveAt(0, Token->TokenCost);
    Player->OwnedHounds.AddUnique(Token->HoundClass);
    return true;
}

bool AWizardWarIGameModeBase::BuyRobe(AWizardPlayerState* Player, const FRobeData& Robe)
{
    if (!Player)
    {
        return false;
    }
    if (Player->TokenInventory.Num() < Robe.TokenCost)
    {
        return false;
    }

    Player->TokenInventory.RemoveAt(0, Robe.TokenCost);
    Player->OwnedRobes.AddUnique(Robe.Name);
    return true;
}

const FRobeData* AWizardWarIGameModeBase::FindRobe(const FString& Name) const
{
    for (const FRobeData& Data : AvailableRobes)
    {
        if (Data.Name.Equals(Name))
        {
            return &Data;
        }
    }
    return nullptr;
}

void AWizardWarIGameModeBase::EquipRobe(AWizardPlayerState* Player, const FString& RobeName)
{
    if (!Player || !Player->OwnedRobes.Contains(RobeName))
    {
        return;
    }

    Player->EquippedRobe = RobeName;

    const FRobeData* Data = FindRobe(RobeName);
    if (Data)
    {
        Player->RobeAttackBonus = Data->AttackBonus;
        Player->RobeShieldBonus = Data->ShieldBonus;
    }
    else
    {
        Player->RobeAttackBonus = 0.f;
        Player->RobeShieldBonus = 0.f;
    }
}

bool AWizardWarIGameModeBase::BuyShoutAttack(AWizardPlayerState* Player, UShoutToken* Token)
{
    if (!Player || !Token)
    {
        return false;
    }
    if (Player->TokenInventory.Num() < Token->TokenCost)
    {
        return false;
    }

    Player->TokenInventory.RemoveAt(0, Token->TokenCost);
    Player->OwnedShoutAttacks.AddUnique(Token);
    return true;
}

void AWizardWarIGameModeBase::EquipShoutAttack(AWizardPlayerState* Player, UShoutToken* Token)
{
    if (!Player || !Token || !Player->OwnedShoutAttacks.Contains(Token))
    {
        return;
    }
    Player->EquippedShoutAttack = Token;
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
    SaveGame->SkinColor = Player->Appearance.SkinColor;
    SaveGame->HairColor = Player->Appearance.HairColor;
    SaveGame->EyeColor = Player->Appearance.EyeColor;
    SaveGame->Height = Player->Appearance.Height;
    SaveGame->BodyType = Player->Appearance.BodyType;
    SaveGame->HairStyle = Player->Appearance.HairStyle;
    SaveGame->RobeName = Player->Appearance.RobeName;
    SaveGame->RobeColor = Player->Appearance.RobeColor;
    SaveGame->SpellLog = Player->SpellLog;
    SaveGame->Achievements = Player->Achievements;
    SaveGame->PreferredLanguage = Player->PreferredLanguage;
    SaveGame->OwnedRobes = Player->OwnedRobes;
    SaveGame->EquippedRobe = Player->EquippedRobe;
    SaveGame->OwnedHounds = Player->OwnedHounds;
    SaveGame->EquippedHound = Player->EquippedHound;
    SaveGame->OwnedShoutAttacks = Player->OwnedShoutAttacks;
    SaveGame->EquippedShoutAttack = Player->EquippedShoutAttack;
    SaveGame->CustomTauntMessage = Player->CustomTauntMessage;

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
        Player->Appearance.SkinColor = SaveGame->SkinColor;
        Player->Appearance.HairColor = SaveGame->HairColor;
        Player->Appearance.EyeColor = SaveGame->EyeColor;
        Player->Appearance.Height = SaveGame->Height;
        Player->Appearance.BodyType = SaveGame->BodyType;
        Player->Appearance.HairStyle = SaveGame->HairStyle;
        Player->Appearance.RobeName = SaveGame->RobeName;
        Player->Appearance.RobeColor = SaveGame->RobeColor;
        Player->SpellLog = SaveGame->SpellLog;
        Player->Achievements = SaveGame->Achievements;
        Player->PreferredLanguage = SaveGame->PreferredLanguage;
        Player->OwnedRobes = SaveGame->OwnedRobes;
        Player->EquippedRobe = SaveGame->EquippedRobe;
        EquipRobe(Player, SaveGame->EquippedRobe);
        Player->OwnedHounds = SaveGame->OwnedHounds;
        Player->EquippedHound = SaveGame->EquippedHound;
        Player->OwnedShoutAttacks = SaveGame->OwnedShoutAttacks;
        Player->EquippedShoutAttack = SaveGame->EquippedShoutAttack;
        Player->CustomTauntMessage = SaveGame->CustomTauntMessage;
        return true;
    }

    return false;
}

void AWizardWarIGameModeBase::SpawnRandomEnvironment()
{
    if (AvailableEnvironments.Num() == 0)
    {
        return;
    }

    int32 Index = FMath::RandRange(0, AvailableEnvironments.Num() - 1);
    const FEnvironmentEntry& Entry = AvailableEnvironments[Index];
    if (Entry.EnvironmentClass)
    {
        FActorSpawnParameters Params;
        ActiveEnvironment = GetWorld()->SpawnActor<AActor>(Entry.EnvironmentClass, Params);
    }
}

void AWizardWarIGameModeBase::StartPlay()
{
    Super::StartPlay();
    if (MenuMusic)
    {
        UGameplayStatics::SpawnSound2D(this, MenuMusic);
    }
    if (HomeCastleClass)
    {
        GetWorld()->SpawnActor<ACastleHub>(HomeCastleClass);
    }
    SpawnRandomEnvironment();
    if (GEngine)
    {
        // Lock the game to 60 FPS for smoother play
        GEngine->Exec(GetWorld(), TEXT("t.MaxFPS 60"));
    }
}
