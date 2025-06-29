#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

    /** Current player level. Determines ability unlocks */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
    int32 PlayerLevel;
};
