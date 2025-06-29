#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpellEffectToken.h"
#include "SpellActor.generated.h"

UCLASS()
class ASpellActor : public AActor
{
    GENERATED_BODY()
public:
    ASpellActor();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UProjectileMovementComponent* Movement;

    /** Configure physics properties based on power, area and effect type */
    void InitSpell(float Power, float Area, ESpellEffectType EffectType);
};
