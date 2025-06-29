#pragma once
#include "CoreMinimal.h"
#include "Token.h"
#include "Animation/AnimMontage.h"
#include "SpellEffectToken.generated.h"

UENUM(BlueprintType)
enum class ESpellEffectType : uint8
{
    Earth,
    Air,
    Fire,
    Water,
    Electricity,

    Weapon,
    /** Explosive spells cause large knockback */
    Explosion,
    /** Freezing spells temporarily immobilise the opponent */
    Freeze
=======
    Weapon

};

UCLASS(Blueprintable)
class USpellEffectToken : public UToken
{
    GENERATED_BODY()
public:
    USpellEffectToken();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
    ESpellEffectType EffectType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
    UAnimMontage* Gesture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
    UAnimMontage* Posture;


    /** Facial expression played while casting */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
    UAnimMontage* FacialExpression;
=======
=======

    /** Facial expression played while casting */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Effect")
    UAnimMontage* FacialExpression;


};
