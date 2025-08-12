#include "WizardCharacter.h"
#include "GameFramework/InputSettings.h"
#include "SpellActor.h"
#include "WizardPlayerState.h"
#include "Camera/CameraComponent.h"
#include "ShieldToken.h"
#include "SpellEffectToken.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CompanionToken.h"
#include "HellHoundCharacter.h"

#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

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
    ActiveCompanion = nullptr;
    SelectedCompanionClass = nullptr;


    LockedOpponent = nullptr;


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

    bLeftTriggerHeld = false;
    bRightTriggerHeld = false;
    bSpeedyActive = false;
    bLeftCasting = false;
    bRightCasting = false;
    bKnockedDown = false;
    OriginalSpeed = 0.f;
    OriginalScale = FVector(1.f);
}

void AWizardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAction("LeftTrigger", IE_Pressed, this, &AWizardCharacter::OnLeftTriggerPressed);
    PlayerInputComponent->BindAction("LeftTrigger", IE_Released, this, &AWizardCharacter::OnLeftTriggerReleased);
    PlayerInputComponent->BindAction("RightTrigger", IE_Pressed, this, &AWizardCharacter::OnRightTriggerPressed);
    PlayerInputComponent->BindAction("RightTrigger", IE_Released, this, &AWizardCharacter::OnRightTriggerReleased);
    PlayerInputComponent->BindAction("LeftTrigger", IE_Pressed, this, &AWizardCharacter::CastLeftArm);
    PlayerInputComponent->BindAction("RightTrigger", IE_Pressed, this, &AWizardCharacter::CastRightArm);
    PlayerInputComponent->BindAction("LeftButton", IE_Pressed, this, &AWizardCharacter::CastLeftArmPower);
    PlayerInputComponent->BindAction("RightButton", IE_Pressed, this, &AWizardCharacter::CastRightArmPower);
    PlayerInputComponent->BindAction("XButton", IE_Pressed, this, &AWizardCharacter::SwitchLeftSlot);
    PlayerInputComponent->BindAction("YButton", IE_Pressed, this, &AWizardCharacter::SwitchRightSlot);
    PlayerInputComponent->BindAction("MenuButton", IE_Pressed, this, &AWizardCharacter::OpenCharacterMenu);
    PlayerInputComponent->BindAction("LeftThumb", IE_Pressed, this, &AWizardCharacter::ShoutTaunt);
}

}


    PlayerInputComponent->BindAction("MenuButton", IE_Pressed, this, &AWizardCharacter::OpenCharacterMenu);
}

}



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

void AWizardCharacter::OnLeftTriggerPressed()
{
    bLeftTriggerHeld = true;
    if (bRightTriggerHeld)
    {
        OnBothTriggersPressed();
        return;
    }
    StartCastLeftArm();
}

void AWizardCharacter::OnLeftTriggerReleased()
{
    bLeftTriggerHeld = false;
    if (!bRightTriggerHeld)
    {
        DeactivateSpeedyRobe();
    }
}

void AWizardCharacter::OnRightTriggerPressed()
{
    bRightTriggerHeld = true;
    if (bLeftTriggerHeld)
    {
        OnBothTriggersPressed();
        return;
    }
    StartCastRightArm();
}

void AWizardCharacter::OnRightTriggerReleased()
{
    bRightTriggerHeld = false;
    if (!bLeftTriggerHeld)
    {
        DeactivateSpeedyRobe();
    }
}

void AWizardCharacter::StartCastLeftArm()
{
    if (bLeftArmLevitation || bLeftArmShield || bLeftCasting || bKnockedDown)
    {
        return;
    }
    bLeftCasting = true;
    GetWorldTimerManager().SetTimer(LeftCastTimer, this, &AWizardCharacter::FinishCastLeftArm, 0.3f, false);
}

void AWizardCharacter::StartCastRightArm()
{
    if (bRightArmLevitation || bRightArmShield || bRightCasting || bKnockedDown)
    {
        return;
    }
    bRightCasting = true;
    GetWorldTimerManager().SetTimer(RightCastTimer, this, &AWizardCharacter::FinishCastRightArm, 0.3f, false);
}

void AWizardCharacter::FinishCastLeftArm()



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
        AddActorWorldOffset(FVector(0.f, 0.f, Lev->PowerValue * 20.f));
        bLeftCasting = false;
        return;
    }
    if (UShieldToken* Shield = Cast<UShieldToken>(Token))
    {
        bLeftArmShield = true;
        AWizardPlayerState* PS = GetPlayerState<AWizardPlayerState>();
        float Bonus = Shield->PowerValue * Shield->DefenseMultiplier;
        if (PS)
        {
            Bonus += PS->RobeShieldBonus;
        }
        ShieldDefenseBonus += Bonus;
        ShieldMesh->SetVisibility(true);
        bLeftCasting = false;
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
                AWizardPlayerState* PS = GetPlayerState<AWizardPlayerState>();
                float Power = Effect->PowerValue;
                float Area = Effect->AreaValue;
                if (PS)
                {
                    Power += PS->RobeAttackBonus;
                }
                if (bSpeedyActive)
                {
                    Power *= 0.1f;
                    Area *= 0.1f;
                }
                Spell->InitSpell(Power, Area, Effect->EffectType);
                if (PS && PS->IsTieDyeRobeEquipped() && Effect->EffectType == ESpellEffectType::Fire && Effect->AreaValue >= 50.f)
                {
                    if (GEngine)
                    {
                        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("I didn't ask how big the room is, I said I cast fireball!"));
                    }
                }
                Spell->InitSpell(Effect->PowerValue, Effect->AreaValue, Effect->EffectType);
            }
        }
        ApplySpellEffectMovement(Effect->EffectType);
        PlayFacialExpression(Effect->FacialExpression);
        ApplyOpponentEffect(Effect->EffectType);
    }
    bLeftCasting = false;
}

void AWizardCharacter::FinishCastRightArm()

        PlayFacialExpression(Effect->FacialExpression);
        ApplyOpponentEffect(Effect->EffectType);

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
        AddActorWorldOffset(FVector(0.f, 0.f, Lev->PowerValue * 20.f));
        bRightCasting = false;
        return;
    }
    if (UShieldToken* Shield = Cast<UShieldToken>(Token))
    {
        bRightArmShield = true;
        AWizardPlayerState* PS = GetPlayerState<AWizardPlayerState>();
        float Bonus = Shield->PowerValue * Shield->DefenseMultiplier;
        if (PS)
        {
            Bonus += PS->RobeShieldBonus;
        }
        ShieldDefenseBonus += Bonus;
        ShieldMesh->SetVisibility(true);
        bRightCasting = false;
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
                AWizardPlayerState* PS = GetPlayerState<AWizardPlayerState>();
                float Power = Effect->PowerValue;
                float Area = Effect->AreaValue;
                if (PS)
                {
                    Power += PS->RobeAttackBonus;
                }
                if (bSpeedyActive)
                {
                    Power *= 0.1f;
                    Area *= 0.1f;
                }
                Spell->InitSpell(Power, Area, Effect->EffectType);
                if (PS && PS->IsTieDyeRobeEquipped() && Effect->EffectType == ESpellEffectType::Fire && Effect->AreaValue >= 50.f)
                {
                    if (GEngine)
                    {
                        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("I didn't ask how big the room is, I said I cast fireball!"));
                    }
                }
                Spell->InitSpell(Effect->PowerValue, Effect->AreaValue, Effect->EffectType);
            }
        }
        ApplySpellEffectMovement(Effect->EffectType);
        PlayFacialExpression(Effect->FacialExpression);
        ApplyOpponentEffect(Effect->EffectType);
    }
    bRightCasting = false;
}

void AWizardCharacter::OnBothTriggersPressed()
{
    AWizardPlayerState* PS = GetPlayerState<AWizardPlayerState>();
    if (!PS)
    {
        return;
    }

    if (PS->EquippedRobe.Equals(TEXT("TieDye")))
    {
        if (UWorld* World = GetWorld())
        {
            FVector Location = GetActorLocation() + GetActorForwardVector() * 100.f;
            FActorSpawnParameters Params;
            ASpellActor* Spell = World->SpawnActor<ASpellActor>(Location, GetActorRotation(), Params);
            if (Spell)
            {
                float Power = 50.f + PS->RobeAttackBonus;
                Spell->InitSpell(Power, 100.f, ESpellEffectType::Fire);
                if (GEngine)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("I didn't ask how big the room is, I said I cast fireball!"));
                }
            }
        }
    }
    else if (PS->EquippedRobe.Equals(TEXT("Speedy")))
    {
        ActivateSpeedyRobe();
    }
}

void AWizardCharacter::ActivateSpeedyRobe()
{
    if (bSpeedyActive)
    {
        return;
    }
    bSpeedyActive = true;
    OriginalSpeed = GetCharacterMovement()->MaxWalkSpeed;
    OriginalScale = GetActorScale3D();
    GetCharacterMovement()->MaxWalkSpeed *= 3.f;
    SetActorScale3D(OriginalScale * 0.1f);
}

void AWizardCharacter::DeactivateSpeedyRobe()
{
    if (!bSpeedyActive)
    {
        return;
    }
    bSpeedyActive = false;
    GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
    SetActorScale3D(OriginalScale);

        PlayFacialExpression(Effect->FacialExpression);
        ApplyOpponentEffect(Effect->EffectType);

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

        LeftSlotIndex = (LeftSlotIndex + 1) % FMath::Max(LeftQuickSlots.Num(), GetMaxArmSlots());
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

        RightSlotIndex = (RightSlotIndex + 1) % FMath::Max(RightQuickSlots.Num(), GetMaxArmSlots());
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

        if (RightQuickSlots.Num() < MaxSlots)
        {
            RightQuickSlots.SetNum(MaxSlots);
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

        case ESpellEffectType::Weapon:
        default:
            break;
    }
}




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

void AWizardCharacter::SummonCompanion()
{
    if (ActiveCompanion || !SelectedCompanionClass)
    {
        return;
    }

    if (UWorld* World = GetWorld())
    {
        FActorSpawnParameters Params;
        Params.Owner = this;
        FVector SpawnLoc = GetActorLocation() + GetActorRightVector() * 100.f;
        ActiveCompanion = World->SpawnActor<AHellHoundCharacter>(SelectedCompanionClass, SpawnLoc, GetActorRotation(), Params);
        if (ActiveCompanion)
        {
            ActiveCompanion->OwnerWizard = this;
        }
    }
}

void AWizardCharacter::DismissCompanion()
{
    if (ActiveCompanion)
    {
        ActiveCompanion->Destroy();
        ActiveCompanion = nullptr;
    }
}

void AWizardCharacter::OnCompanionKilled(AHellHoundCharacter* DeadCompanion)
{
    if (ActiveCompanion == DeadCompanion)
    {
        ActiveCompanion = nullptr;
        // Immediately spawn a replacement if a class is selected
        SummonCompanion();
    }
}

void AWizardCharacter::OpenCharacterMenu()
{
    // Implementation will be provided in Blueprint to show the menu widget
}

void AWizardCharacter::ShoutTaunt()
{
    AWizardPlayerState* PS = GetPlayerState<AWizardPlayerState>();
    if (PS && GEngine)
    {
        FString Text = PS->GetCensoredTaunt();
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, Text);
    }
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
            if (AWizardCharacter* Wiz = Cast<AWizardCharacter>(OppChar))
            {
                Wiz->HandleKnockDown();
            }
            break;
        case ESpellEffectType::Fire:
            // A real project would play a burn animation here
            break;
        case ESpellEffectType::Explosion:
            OppChar->LaunchCharacter((OppChar->GetActorLocation() - GetActorLocation()).GetSafeNormal() * 500.f, true, true);
            if (AWizardCharacter* Wiz = Cast<AWizardCharacter>(OppChar))
            {
                Wiz->HandleKnockDown();
            }
            break;
        default:
            break;
    }
}

void AWizardCharacter::HandleKnockDown()
{
    bKnockedDown = true;
    bLeftCasting = false;
    bRightCasting = false;
    GetWorldTimerManager().ClearTimer(LeftCastTimer);
    GetWorldTimerManager().ClearTimer(RightCastTimer);
    GetWorldTimerManager().SetTimer(KnockDownTimer, this, &AWizardCharacter::RecoverFromKnockDown, 1.0f, false);
}

void AWizardCharacter::RecoverFromKnockDown()
{
    bKnockedDown = false;
}

