#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WizardPlayerState.h"
#include "CharacterCreationWidget.generated.h"

/**
 * Basic character creation menu allowing players to customise appearance.
 * This is only a stub; actual UI logic must be implemented in Blueprint.
 */
UCLASS()
class UCharacterCreationWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    /** Current preview of the player's appearance settings */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Appearance")
    FWizardAppearance PreviewAppearance;

    /** Apply the preview settings to the provided player state */
    UFUNCTION(BlueprintCallable, Category="Appearance")
    void ApplyToPlayerState(AWizardPlayerState* PlayerState);
};
