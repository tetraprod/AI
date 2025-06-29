#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsWidget.generated.h"

/**
 * Settings menu for adjusting controls, audio and video.
 * Actual UI is implemented in Blueprint.
 */
UCLASS()
class USettingsWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    /** Remap a controller action to a new key */
    UFUNCTION(BlueprintCallable, Category="Controls")
    void RemapAction(FName ActionName, FKey NewKey);

    /** Adjust master audio volume (0-1) */
    UFUNCTION(BlueprintCallable, Category="Audio")
    void SetMasterVolume(float Volume);

    /** Change screen resolution and fullscreen state */
    UFUNCTION(BlueprintCallable, Category="Video")
    void SetResolution(FIntPoint Resolution, bool bFullscreen);
};
