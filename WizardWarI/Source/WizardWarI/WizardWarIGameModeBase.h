#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
=======

=======


#include "Sound/SoundBase.h"
#include "WizardWarIGameModeBase.generated.h"

UCLASS()
=======

=======
=======
#include "WizardWarIGameModeBase.generated.h"

UCLASS()



class AWizardPlayerState;

class AWizardWarIGameModeBase : public AGameModeBase
{
    GENERATED_BODY()
public:
    /** Host a bet match placing tokens as the wager */
    UFUNCTION(BlueprintCallable, Category="Multiplayer")
    bool HostBetMatch(AWizardPlayerState* HostPlayer, const TArray<UToken*>& Tokens);

    /** Join an existing bet match if the wager matches */
    UFUNCTION(BlueprintCallable, Category="Multiplayer")
    bool JoinBetMatch(AWizardPlayerState* JoiningPlayer, const TArray<UToken*>& Tokens);

    /** Resolve the bet and award tokens to the winner */
    UFUNCTION(BlueprintCallable, Category="Multiplayer")
    void ResolveBet(AWizardPlayerState* Winner, AWizardPlayerState* Loser);

=======

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
=======
=======
    /** Grant experience based on match duration */
    UFUNCTION(BlueprintCallable, Category="Gameplay")
    void AwardMatchXP(AWizardPlayerState* Player, float MatchLengthSeconds);

    /** Save the given player's state to a slot */
    UFUNCTION(BlueprintCallable, Category="Save")
    bool SavePlayer(AWizardPlayerState* Player, const FString& SlotName);

    /** Load the given player's state from a slot */
    UFUNCTION(BlueprintCallable, Category="Save")
    bool LoadPlayer(AWizardPlayerState* Player, const FString& SlotName);

protected:
    /** Pending host waiting for a challenger */
    UPROPERTY()
    AWizardPlayerState* PendingHost;

    /** Tokens wagered by the host */
    UPROPERTY()
    TArray<UToken*> PendingWager;
=======
class AWizardWarIGameModeBase : public AGameModeBase
{
    GENERATED_BODY()
=======

class AWizardPlayerState;

class AWizardWarIGameModeBase : public AGameModeBase
{
    GENERATED_BODY()
public:
    /** Host a bet match placing tokens as the wager */
    UFUNCTION(BlueprintCallable, Category="Multiplayer")
    bool HostBetMatch(AWizardPlayerState* HostPlayer, const TArray<UToken*>& Tokens);

    /** Join an existing bet match if the wager matches */
    UFUNCTION(BlueprintCallable, Category="Multiplayer")
    bool JoinBetMatch(AWizardPlayerState* JoiningPlayer, const TArray<UToken*>& Tokens);

    /** Resolve the bet and award tokens to the winner */
    UFUNCTION(BlueprintCallable, Category="Multiplayer")
    void ResolveBet(AWizardPlayerState* Winner, AWizardPlayerState* Loser);


    /** Grant experience based on match duration */
    UFUNCTION(BlueprintCallable, Category="Gameplay")
    void AwardMatchXP(AWizardPlayerState* Player, float MatchLengthSeconds);

    /** Save the given player's state to a slot */
    UFUNCTION(BlueprintCallable, Category="Save")
    bool SavePlayer(AWizardPlayerState* Player, const FString& SlotName);

    /** Load the given player's state from a slot */
    UFUNCTION(BlueprintCallable, Category="Save")
    bool LoadPlayer(AWizardPlayerState* Player, const FString& SlotName);

=======

=======


    virtual void StartPlay() override;

    /** Background music played on the main menu */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
    class USoundBase* MenuMusic;

=======

=======
=======


protected:
    /** Pending host waiting for a challenger */
    UPROPERTY()
    AWizardPlayerState* PendingHost;

    /** Tokens wagered by the host */
    UPROPERTY()
    TArray<UToken*> PendingWager;

=======

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
=======
=======
=======
=======
class AWizardWarIGameModeBase : public AGameModeBase
{
    GENERATED_BODY()




};
