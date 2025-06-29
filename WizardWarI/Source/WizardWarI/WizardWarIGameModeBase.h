#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
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
};
