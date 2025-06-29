#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Token.h"
#include "LevitationToken.h"
#include "AreaToken.h"
#include "ShieldToken.h"
#include "SpellEffectToken.h"
#include "CompanionToken.h"
#include "HellHoundCharacter.h"
#include "Animation/AnimMontage.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "WizardCharacter.generated.h"

UCLASS()
class AWizardCharacter : public ACharacter
{
    GENERATED_BODY()
public:
    AWizardCharacter();

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void Tick(float DeltaSeconds) override;

    UFUNCTION()
    void FinishCastLeftArm();

    UFUNCTION()
    void OnLeftTriggerPressed();

    UFUNCTION()
    void OnLeftTriggerReleased();

    UFUNCTION()
    void FinishCastRightArm();

    UFUNCTION()
    void OnRightTriggerPressed();

    UFUNCTION()
    void OnRightTriggerReleased();

    /** Triggered when both triggers are pressed together */
    void OnBothTriggersPressed();

    /** Begin casting with delay so knockdown can interrupt */
    void StartCastLeftArm();
    void FinishCastLeftArm();
    void StartCastRightArm();
    void FinishCastRightArm();

    /** Handle knockdown interruptions */
    void HandleKnockDown();
    void RecoverFromKnockDown();

    /** Speedy robe helpers */
    void ActivateSpeedyRobe();
    void DeactivateSpeedyRobe();

    UFUNCTION()
    void CastLeftArmPower();

    UFUNCTION()
    void CastRightArmPower();

    /** Open the character menu using the Menu button */
    UFUNCTION()
    void OpenCharacterMenu();

    /** Perform the equipped taunt */
    UFUNCTION()
    void ShoutTaunt();

    /** Swap quick slot for the left arm (requires level >=5) */
    UFUNCTION()
    void SwitchLeftSlot();

    /** Swap quick slot for the right arm (requires level >=5) */
    UFUNCTION()
    void SwitchRightSlot();

    /** Assign a token to a quick slot if unlocked */
    void AssignTokenToQuickSlot(UToken* Token, bool bLeftArm, int32 SlotIndex);

    /** Maximum quick slots available for the player's current level */
    int32 GetMaxArmSlots() const;


    /** Quick slot arrays unlocked at level 5 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tokens")
    TArray<UToken*> LeftQuickSlots;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tokens")
    TArray<UToken*> RightQuickSlots;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tokens")
    int32 LeftSlotIndex;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Tokens")
    int32 RightSlotIndex;

    /** Whether the left or right arm is occupied by a levitation spell */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Levitation")
    bool bLeftArmLevitation;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Levitation")
    bool bRightArmLevitation;

    /** Movement speed bonus granted by levitation */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Levitation")
    float LevitationSpeedBonus;

    /** Whether the left or right arm is occupied by a shield spell */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Shield")
    bool bLeftArmShield;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Shield")
    bool bRightArmShield;

    /** Damage reduction bonus granted by shielding */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Shield")
    float ShieldDefenseBonus;

    /** Visible barrier mesh shown when shielded */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Shield")
    UStaticMeshComponent* ShieldMesh;

    /** Whether each trigger is currently held */
    bool bLeftTriggerHeld;

    bool bRightTriggerHeld;

    /** If the Speedy robe effect is active */
    bool bSpeedyActive;

    /** True while the left or right arm is charging a spell */
    bool bLeftCasting;
    bool bRightCasting;

    /** True if the character has been knocked down and is recovering */
    bool bKnockedDown;

    FTimerHandle LeftCastTimer;
    FTimerHandle RightCastTimer;
    FTimerHandle KnockDownTimer;

    /** Cached normal values for Speedy robe */
    float OriginalSpeed;
    FVector OriginalScale;

    /** First person camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
    UCameraComponent* FirstPersonCamera;

    /** Widget class for the character menu */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
    TSubclassOf<class UUserWidget> CharacterMenuClass;

    /** Current opponent actor for keeping eyes locked */
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Facial")
    AActor* LockedOpponent;

    /** Assign the actor the character should keep looking at */
    UFUNCTION(BlueprintCallable, Category="Facial")
    void SetOpponent(AActor* Opponent);

    /** Play a facial expression montage */
    UFUNCTION(BlueprintCallable, Category="Facial")
    void PlayFacialExpression(UAnimMontage* Expression);

    /** Spawn the currently selected companion */
    UFUNCTION(BlueprintCallable, Category="Companion")
    void SummonCompanion();

    /** Remove the companion from the field */
    UFUNCTION(BlueprintCallable, Category="Companion")
    void DismissCompanion();

    /** Choose which companion to use between rounds */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Companion")
    TSubclassOf<AHellHoundCharacter> SelectedCompanionClass;

    /** Active companion actor */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Companion")
    AHellHoundCharacter* ActiveCompanion;

    /** Called by the hound when it dies so a backup can spawn */
    UFUNCTION()
    void OnCompanionKilled(AHellHoundCharacter* DeadCompanion);

protected:
    /** Apply movement or posture changes for a spell effect */
    void ApplySpellEffectMovement(ESpellEffectType EffectType);

    /** Apply a spell reaction to the locked opponent */
    void ApplyOpponentEffect(ESpellEffectType EffectType);
};
