#include "WizardCharacter.h"
#include "GameFramework/InputSettings.h"
#include "SpellActor.h"
#include "WizardPlayerState.h"
#include "Camera/CameraComponent.h"
#include "ShieldToken.h"
#include "SpellEffectToken.h"

#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
=======

=======
#include "Kismet/KismetMathLibrary.h"



AWizardCharacter::AWizardCharacter()
{
    LeftSlotIndex = 0;
    RightSlotIndex = 0;
    bLeftArmLevitation = false;
    bRightArmLevitation = false;
    LevitationSpeedBonus = 0.f;
    bLeftArmShield = false;
    bRightArmShield = false;
    ShieldDefenseBonus = 0.f;

    LockedOpponent = nullptr;

=======

=======
    LockedOpponent = nullptr;



    ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
    ShieldMesh->SetupAttachment(RootComponent);
    ShieldMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    ShieldMesh->SetVisibility(false);
    ShieldMesh->SetRelativeScale3D(FVector(2.f));

    FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
    FirstPersonCamera->SetRelativeLocation(FVector(0.f, 0.f, 64.f));
    FirstPersonCamera->bUsePawnControlRotation = true;

    bUseControllerRotationYaw = true;
}

void AWizardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAction("LeftTrigger", IE_Pressed, this, &AWizardCharacter::CastLeftArm);
    PlayerInputComponent->BindAction("RightTrigger", IE_Pressed, this, &AWizardCharacter::CastRightArm);
    PlayerInputComponent->BindAction("LeftButton", IE_Pressed, this, &AWizardCharacter::CastLeftArmPower);
    PlayerInputComponent->BindAction("RightButton", IE_Pressed, this, &AWizardCharacter::CastRightArmPower);
    PlayerInputComponent->BindAction("XButton", IE_Pressed, this, &AWizardCharacter::SwitchLeftSlot);
    PlayerInputComponent->BindAction("YButton", IE_Pressed, this, &AWizardCharacter::SwitchRightSlot);

    PlayerInputComponent->BindAction("MenuButton", IE_Pressed, this, &AWizardCharacter::OpenCharacterMenu);
}

=======
}


=======

void AWizardCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (LockedOpponent)
    {
        FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockedOpponent->GetActorLocation());
        FRotator NewRot = FRotator(GetActorRotation().Pitch, LookAtRot.Yaw, GetActorRotation().Roll);
        GetMesh()->SetWorldRotation(NewRot);
    }
}


=======


void AWizardCharacter::CastLeftArm()
{
    if (bLeftArmLevitation || bLeftArmShield)
    {
        return; // arm is occupied by another persistent effect
    }

    UToken* Token = LeftQuickSlots.IsValidIndex(LeftSlotIndex) ? LeftQuickSlots[LeftSlotIndex] : nullptr;
    if (ULevitationToken* Lev = Cast<ULevitationToken>(Token))
    {
        bLeftArmLevitation = true;
        float Bonus = Lev->PowerValue * Lev->SpeedMultiplier;
        LevitationSpeedBonus += Bonus;
        GetCharacterMovement()->MaxWalkSpeed += Bonus;
        return;
    }
    if (UShieldToken* Shield = Cast<UShieldToken>(Token))
    {
        bLeftArmShield = true;
        float Bonus = Shield->PowerValue * Shield->DefenseMultiplier;
        ShieldDefenseBonus += Bonus;
        ShieldMesh->SetVisibility(true);
        return;
    }

    if (USpellEffectToken* Effect = Cast<USpellEffectToken>(Token))
    {
        if (UWorld* World = GetWorld())
        {
            FVector Location = GetActorLocation() + GetActorForwardVector() * 100.f;
            FActorSpawnParameters Params;
            ASpellActor* Spell = World->SpawnActor<ASpellActor>(Location, GetActorRotation(), Params);
            if (Spell)
            {
                Spell->InitSpell(Effect->PowerValue, Effect->AreaValue, Effect->EffectType);
            }
        }
        ApplySpellEffectMovement(Effect->EffectType);

        PlayFacialExpression(Effect->FacialExpression);
        ApplyOpponentEffect(Effect->EffectType);
=======

=======
        PlayFacialExpression(Effect->FacialExpression);


    }
}

void AWizardCharacter::CastRightArm()
{
    if (bRightArmLevitation || bRightArmShield)
    {
        return; // arm is occupied by another persistent effect
    }

    UToken* Token = RightQuickSlots.IsValidIndex(RightSlotIndex) ? RightQuickSlots[RightSlotIndex] : nullptr;
    if (ULevitationToken* Lev = Cast<ULevitationToken>(Token))
    {
        bRightArmLevitation = true;
        float Bonus = Lev->PowerValue * Lev->SpeedMultiplier;
        LevitationSpeedBonus += Bonus;
        GetCharacterMovement()->MaxWalkSpeed += Bonus;
        return;
    }
    if (UShieldToken* Shield = Cast<UShieldToken>(Token))
    {
        bRightArmShield = true;
        float Bonus = Shield->PowerValue * Shield->DefenseMultiplier;
        ShieldDefenseBonus += Bonus;
        ShieldMesh->SetVisibility(true);
        return;
    }

    if (USpellEffectToken* Effect = Cast<USpellEffectToken>(Token))
    {
        if (UWorld* World = GetWorld())
        {
            FVector Location = GetActorLocation() + GetActorForwardVector() * 100.f;
            FActorSpawnParameters Params;
            ASpellActor* Spell = World->SpawnActor<ASpellActor>(Location, GetActorRotation(), Params);
            if (Spell)
            {
                Spell->InitSpell(Effect->PowerValue, Effect->AreaValue, Effect->EffectType);
            }
        }
        ApplySpellEffectMovement(Effect->EffectType);

        PlayFacialExpression(Effect->FacialExpression);
        ApplyOpponentEffect(Effect->EffectType);
=======

=======
        PlayFacialExpression(Effect->FacialExpression);


    }
}

void AWizardCharacter::CastLeftArmPower()
{
    AWizardPlayerState* PS = GetPlayerState<AWizardPlayerState>();
    int32 Level = PS ? PS->GetLevel() : 1;
    if (Level >= 10)
    {
        // Cast left arm with double effect
    }
}

void AWizardCharacter::CastRightArmPower()
{
    AWizardPlayerState* PS = GetPlayerState<AWizardPlayerState>();
    int32 Level = PS ? PS->GetLevel() : 1;
    if (Level >= 10)
    {
        // Cast right arm with double effect
    }
}

void AWizardCharacter::SwitchLeftSlot()
{
    AWizardPlayerState* PS = GetPlayerState<AWizardPlayerState>();
    int32 Level = PS ? PS->GetLevel() : 1;
    if (Level >= 5 && !bLeftArmLevitation && !bLeftArmShield && LeftQuickSlots.Num() > 0)
    {

        LeftSlotIndex = (LeftSlotIndex + 1) % FMath::Max(LeftQuickSlots.Num(), GetMaxArmSlots());
=======
        LeftSlotIndex = (LeftSlotIndex + 1) % LeftQuickSlots.Num();

    }
}

void AWizardCharacter::SwitchRightSlot()
{
    AWizardPlayerState* PS = GetPlayerState<AWizardPlayerState>();
    int32 Level = PS ? PS->GetLevel() : 1;
    if (Level >= 5 && !bRightArmLevitation && !bRightArmShield && RightQuickSlots.Num() > 0)
    {

        RightSlotIndex = (RightSlotIndex + 1) % FMath::Max(RightQuickSlots.Num(), GetMaxArmSlots());
=======
        RightSlotIndex = (RightSlotIndex + 1) % RightQuickSlots.Num();

    }
}

void AWizardCharacter::AssignTokenToQuickSlot(UToken* Token, bool bLeftArm, int32 SlotIndex)
{
    AWizardPlayerState* PS = GetPlayerState<AWizardPlayerState>();
    int32 Level = PS ? PS->GetLevel() : 1;
    if (Level < 5 || !Token)
    {
        return;
    }


    int32 MaxSlots = GetMaxArmSlots();
    if (SlotIndex >= MaxSlots)
    {
        return;
    }

    if (bLeftArm)
    {
        if (LeftQuickSlots.Num() < MaxSlots)
        {
            LeftQuickSlots.SetNum(MaxSlots);
=======
    if (bLeftArm)
    {
        if (LeftQuickSlots.Num() <= SlotIndex)
        {
            LeftQuickSlots.SetNum(SlotIndex + 1);

        }
        LeftQuickSlots[SlotIndex] = Token;
    }
    else
    {

        if (RightQuickSlots.Num() < MaxSlots)
        {
            RightQuickSlots.SetNum(MaxSlots);
=======
        if (RightQuickSlots.Num() <= SlotIndex)
        {
            RightQuickSlots.SetNum(SlotIndex + 1);

        }
        RightQuickSlots[SlotIndex] = Token;
    }
}

void AWizardCharacter::ApplySpellEffectMovement(ESpellEffectType EffectType)
{
    switch (EffectType)
    {
        case ESpellEffectType::Earth:
            LaunchCharacter(FVector(0.f, 0.f, -50.f), false, false);
            break;
        case ESpellEffectType::Air:
            LaunchCharacter(FVector(0.f, 0.f, 75.f), false, false);
            break;
        case ESpellEffectType::Fire:
            LaunchCharacter(-GetActorForwardVector() * 50.f, false, false);
            break;
        case ESpellEffectType::Water:
            GetCharacterMovement()->MaxWalkSpeed *= 0.95f;
            break;
        case ESpellEffectType::Electricity:
            GetCharacterMovement()->MaxWalkSpeed *= 1.05f;
            break;

        case ESpellEffectType::Explosion:
            LaunchCharacter(GetActorForwardVector() * -200.f + FVector(0.f,0.f,200.f), true, true);
            break;
        case ESpellEffectType::Freeze:
            // Caster experiences little movement for freeze
            break;
=======

        case ESpellEffectType::Weapon:
        default:
            break;
    }
}


=======
=======


void AWizardCharacter::SetOpponent(AActor* Opponent)
{
    LockedOpponent = Opponent;
}

void AWizardCharacter::PlayFacialExpression(UAnimMontage* Expression)
{
    if (Expression)
    {
        PlayAnimMontage(Expression);
    }
}


void AWizardCharacter::OpenCharacterMenu()
{
    // Implementation will be provided in Blueprint to show the menu widget
}

int32 AWizardCharacter::GetMaxArmSlots() const
{
    const AWizardPlayerState* PS = GetPlayerState<AWizardPlayerState>();
    int32 Level = PS ? PS->GetLevel() : 1;
    return 3 + (Level / 10) * 3;
}

void AWizardCharacter::ApplyOpponentEffect(ESpellEffectType EffectType)
{
    ACharacter* OppChar = Cast<ACharacter>(LockedOpponent);
    if (!OppChar)
    {
        return;
    }

    switch (EffectType)
    {
        case ESpellEffectType::Freeze:
        case ESpellEffectType::Water:
            if (UCharacterMovementComponent* Move = OppChar->GetCharacterMovement())
            {
                Move->DisableMovement();
            }
            break;
        case ESpellEffectType::Electricity:
            OppChar->LaunchCharacter(FVector(0.f, 0.f, -200.f), false, false);
            break;
        case ESpellEffectType::Fire:
            // A real project would play a burn animation here
            break;
        case ESpellEffectType::Explosion:
            OppChar->LaunchCharacter((OppChar->GetActorLocation() - GetActorLocation()).GetSafeNormal() * 500.f, true, true);
            break;
        default:
            break;
    }
}
=======

