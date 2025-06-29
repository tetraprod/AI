#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Token.h"
#include "LevitationToken.h"
#include "ShieldToken.h"
#include "SpellEffectToken.h"
#include "Components/StaticMeshComponent.h"
#include "WizardCharacter.generated.h"

UCLASS()
class AWizardCharacter : public ACharacter
{
    GENERATED_BODY()
public:
    AWizardCharacter();

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION()
    void CastLeftArm();

    UFUNCTION()
    void CastRightArm();

    UFUNCTION()
    void CastLeftArmPower();

    UFUNCTION()
    void CastRightArmPower();

    /** Swap quick slot for the left arm (requires level >=5) */
    UFUNCTION()
    void SwitchLeftSlot();

    /** Swap quick slot for the right arm (requires level >=5) */
    UFUNCTION()
    void SwitchRightSlot();

    /** Assign a token to a quick slot if unlocked */
    void AssignTokenToQuickSlot(UToken* Token, bool bLeftArm, int32 SlotIndex);


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

    /** First person camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
    UCameraComponent* FirstPersonCamera;

protected:
    /** Apply movement or posture changes for a spell effect */
    void ApplySpellEffectMovement(ESpellEffectType EffectType);
};
