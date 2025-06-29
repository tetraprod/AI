#include "SpellActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"
=======

#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"
=======

#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"
=======


=======
#include "Components/PointLightComponent.h"
#include "Kismet/GameplayStatics.h"
=======



ASpellActor::ASpellActor()
{
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;
    Mesh->SetSimulatePhysics(true);

=======
=======

=======

=======


    Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
    Light->SetupAttachment(RootComponent);
    Light->Intensity = 3000.f;
    Light->bUseInverseSquaredFalloff = false;
    Light->AttenuationRadius = 250.f;

=======
=======

=======
=======



    Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
    Movement->InitialSpeed = 1000.f;
    Movement->MaxSpeed = 1000.f;
}

void ASpellActor::InitSpell(float Power, float Area, ESpellEffectType EffectType)
{
    float Scale = FMath::Clamp(Area, 0.5f, 3.f);
    Mesh->SetWorldScale3D(FVector(Scale));
=======
=======

=======

    Movement->Velocity = GetActorForwardVector() * (1000.f + Power * 100.f);

=======


    Mesh->SetMassOverrideInKg(NAME_None, 10.f + Power * 5.f);
    Movement->Velocity = GetActorForwardVector() * (1000.f + Power * 100.f);
    Light->SetIntensity(3000.f + Power * 200.f);

    if (CastSound)
    {
        UGameplayStatics::SpawnSoundAtLocation(this, CastSound, GetActorLocation());
    }

=======
=======
=======
=======
    Movement->Velocity = GetActorForwardVector() * (1000.f + Power * 100.f);




    switch (EffectType)
    {
        case ESpellEffectType::Earth:
            Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.4f, 0.2f, 0.f));
=======
=======

=======

            break;
        case ESpellEffectType::Air:
            Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.8f, 0.8f, 1.f));
            break;
        case ESpellEffectType::Fire:
            Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(1.f, 0.3f, 0.f));
            break;
        case ESpellEffectType::Water:
            Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.f, 0.4f, 0.8f));
            break;
        case ESpellEffectType::Electricity:
            Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(1.f, 1.f, 0.f));
            break;
        case ESpellEffectType::Weapon:
        default:
=======


            Light->SetLightColor(FLinearColor(0.4f, 0.2f, 0.f));
            break;
        case ESpellEffectType::Air:
            Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.8f, 0.8f, 1.f));
            Light->SetLightColor(FLinearColor(0.8f, 0.8f, 1.f));
            break;
        case ESpellEffectType::Fire:
            Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(1.f, 0.3f, 0.f));
            Light->SetLightColor(FLinearColor(1.f, 0.3f, 0.f));
            break;
        case ESpellEffectType::Water:
            Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.f, 0.4f, 0.8f));
            Light->SetLightColor(FLinearColor(0.f, 0.4f, 0.8f));
            break;
        case ESpellEffectType::Electricity:
            Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(1.f, 1.f, 0.f));
            Light->SetLightColor(FLinearColor(1.f, 1.f, 0.f));
            break;
=======
=======

        case ESpellEffectType::Explosion:
            Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(1.f, 0.5f, 0.2f));
            Light->SetLightColor(FLinearColor(1.f, 0.5f, 0.2f));
            break;
        case ESpellEffectType::Freeze:
            Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.5f, 0.8f, 1.f));
            Light->SetLightColor(FLinearColor(0.5f, 0.8f, 1.f));
            break;
        case ESpellEffectType::Weapon:
        default:
            Light->SetLightColor(FLinearColor::White);
=======
=======
=======
        case ESpellEffectType::Weapon:
        default:
            Light->SetLightColor(FLinearColor::White);
=======
            break;
        case ESpellEffectType::Air:
            Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.8f, 0.8f, 1.f));
            break;
        case ESpellEffectType::Fire:
            Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(1.f, 0.3f, 0.f));
            break;
        case ESpellEffectType::Water:
            Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(0.f, 0.4f, 0.8f));
            break;
        case ESpellEffectType::Electricity:
            Mesh->SetVectorParameterValueOnMaterials(TEXT("Color"), FVector(1.f, 1.f, 0.f));
            break;
        case ESpellEffectType::Weapon:
        default:


            break;
    }
}
