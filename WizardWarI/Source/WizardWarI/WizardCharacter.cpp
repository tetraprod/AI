#include "WizardCharacter.h"
#include "GameFramework/InputSettings.h"

AWizardCharacter::AWizardCharacter()
{
    PlayerLevel = 1;
}

void AWizardCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAction("LeftTrigger", IE_Pressed, this, &AWizardCharacter::CastLeftArm);
    PlayerInputComponent->BindAction("RightTrigger", IE_Pressed, this, &AWizardCharacter::CastRightArm);
    PlayerInputComponent->BindAction("LeftButton", IE_Pressed, this, &AWizardCharacter::CastLeftArmPower);
    PlayerInputComponent->BindAction("RightButton", IE_Pressed, this, &AWizardCharacter::CastRightArmPower);
}

void AWizardCharacter::CastLeftArm()
{
    // Trigger casting for tokens assigned to the left arm
}

void AWizardCharacter::CastRightArm()
{
    // Trigger casting for tokens assigned to the right arm
}

void AWizardCharacter::CastLeftArmPower()
{
    if (PlayerLevel >= 10)
    {
        // Cast left arm with double effect
    }
}

void AWizardCharacter::CastRightArmPower()
{
    if (PlayerLevel >= 10)
    {
        // Cast right arm with double effect
    }
}
