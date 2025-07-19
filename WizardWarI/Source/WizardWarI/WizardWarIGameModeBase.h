#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Sound/SoundBase.h"
#include "Engine/DataTable.h"
#include "ShoutToken.h"
#include "CastleHub.h"
#include "GameFramework/Actor.h"
#include "WizardWarIGameModeBase.generated.h"

class AWizardPlayerState;
class UToken;
class UCompanionToken;

USTRUCT(BlueprintType)
struct FRobeData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackBonus = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ShieldBonus = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TokenCost = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSpeedy = false;
};

UENUM(BlueprintType)
enum class ECombatEnvironment : uint8
{
    Dungeon UMETA(DisplayName="Dungeon"),
    Forest UMETA(DisplayName="Forest"),
    ShrinkingIsland UMETA(DisplayName="ShrinkingIsland"),
    Colosseum UMETA(DisplayName="Colosseum"),
    MountainTop UMETA(DisplayName="MountainTop")
};

USTRUCT(BlueprintType)
struct FEnvironmentEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECombatEnvironment Type = ECombatEnvironment::Dungeon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> EnvironmentClass;
};

UCLASS()
class AWizardWarIGameModeBase : public AGameModeBase
{
    GENERATED_BODY()
public:
    AWizardWarIGameModeBase();

    /** Choose and spawn a random combat environment */
    UFUNCTION(BlueprintCallable, Category="Environment")
    void SpawnRandomEnvironment();
    /** Host a bet match placing tokens as the wager */
    UFUNCTION(BlueprintCallable, Category="Multiplayer")
    bool HostBetMatch(AWizardPlayerState* HostPlayer, const TArray<UToken*>& Tokens);

    /** Join an existing bet match if the wager matches */
    UFUNCTION(BlueprintCallable, Category="Multiplayer")
    bool JoinBetMatch(AWizardPlayerState* JoiningPlayer, const TArray<UToken*>& Tokens);

    /** Resolve the bet and award tokens to the winner */
    UFUNCTION(BlueprintCallable, Category="Multiplayer")
    void ResolveBet(AWizardPlayerState* Winner, AWizardPlayerState* Loser);

    /** Begin a multiplayer arena battle for up to twenty players */
    UFUNCTION(BlueprintCallable, Category="Arena")
    bool StartArenaBattle();

    /** Join the active arena battle with a set of wagered tokens */
    UFUNCTION(BlueprintCallable, Category="Arena")
    bool JoinArenaBattle(AWizardPlayerState* Player, const TArray<UToken*>& Tokens);

    /** End the arena battle and reward surviving players */
    UFUNCTION(BlueprintCallable, Category="Arena")
    void EndArenaBattle(const TArray<AWizardPlayerState*>& Survivors);

    /** Award the accumulated pool to the winner of the daily deathmatch */
    UFUNCTION(BlueprintCallable, Category="Arena")
    void ResolveDailyDeathmatch(AWizardPlayerState* Winner);

    /** Grant experience based on match duration */
    UFUNCTION(BlueprintCallable, Category="Gameplay")
    void AwardMatchXP(AWizardPlayerState* Player, float MatchLengthSeconds);

    /** Spend tokens to buy a hell hound companion */
    UFUNCTION(BlueprintCallable, Category="Store")
    bool BuyHellHound(AWizardPlayerState* Player, UCompanionToken* Token);

    /** Spend tokens to purchase a robe */
    UFUNCTION(BlueprintCallable, Category="Store")
    bool BuyRobe(AWizardPlayerState* Player, const FRobeData& Robe);

    /** Equip one of the player's owned robes */
    UFUNCTION(BlueprintCallable, Category="Store")
    void EquipRobe(AWizardPlayerState* Player, const FString& RobeName);

    /** Purchase a shout attack */
    UFUNCTION(BlueprintCallable, Category="Store")
    bool BuyShoutAttack(AWizardPlayerState* Player, UShoutToken* Token);

    /** Equip a purchased shout attack */
    UFUNCTION(BlueprintCallable, Category="Store")
    void EquipShoutAttack(AWizardPlayerState* Player, UShoutToken* Token);

    /** Save the given player's state to a slot */
    UFUNCTION(BlueprintCallable, Category="Save")
    bool SavePlayer(AWizardPlayerState* Player, const FString& SlotName);

    /** Load the given player's state from a slot */
    UFUNCTION(BlueprintCallable, Category="Save")
    bool LoadPlayer(AWizardPlayerState* Player, const FString& SlotName);

    virtual void StartPlay() override;

    /** Background music played on the main menu */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
    class USoundBase* MenuMusic;

    /** Actor class for the player's home castle */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Home")
    TSubclassOf<ACastleHub> HomeCastleClass;

    /** Robes players can purchase from the store */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Store")
    TArray<FRobeData> AvailableRobes;

    /** Shout attacks for sale */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Store")
    TArray<UShoutToken*> AvailableShouts;

    /** Combat arenas to choose from */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Environment")
    TArray<FEnvironmentEntry> AvailableEnvironments;

    /** Currently active environment actor */
    UPROPERTY()
    AActor* ActiveEnvironment = nullptr;

    /** Look up robe data by name */
    const FRobeData* FindRobe(const FString& Name) const;

protected:
    /** Pending host waiting for a challenger */
    UPROPERTY()
    AWizardPlayerState* PendingHost;

    /** Tokens wagered by the host */
    UPROPERTY()
    TArray<UToken*> PendingWager;

    /** Active arena state */
    UPROPERTY()
    bool bArenaActive = false;

    /** Start time of the current arena match */
    UPROPERTY()
    float ArenaStartTime = 0.f;

    /** All players participating in the arena */
    UPROPERTY()
    TArray<AWizardPlayerState*> ArenaPlayers;

    /** Token pool accumulated from arena wagers */
    UPROPERTY()
    TArray<UToken*> ArenaPool;
};
