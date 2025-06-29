#include "CharacterCreationWidget.h"
#include "WizardPlayerState.h"

void UCharacterCreationWidget::ApplyToPlayerState(AWizardPlayerState* PlayerState)
{
    if (PlayerState)
    {
        PlayerState->Appearance = PreviewAppearance;
    }
}
