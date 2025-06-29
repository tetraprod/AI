#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpellEffectToken.h"

#include "Components/PointLightComponent.h"
#include "Sound/SoundBase.h"
=======

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


    /** Dynamic light that follows the spell for improved visuals */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UPointLightComponent* Light;

    /** Sound cue played when the spell is spawned */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
    class USoundBase* CastSound;

=======

    /** Configure physics properties based on power, area and effect type */
    void InitSpell(float Power, float Area, ESpellEffectType EffectType);
};
