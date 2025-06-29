#include "SettingsWidget.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameUserSettings.h"
#include "Engine/Engine.h"

void USettingsWidget::RemapAction(FName ActionName, FKey NewKey)
{
    if (UInputSettings* Settings = const_cast<UInputSettings*>(GetDefault<UInputSettings>()))
    {
        TArray<FInputActionKeyMapping> Mappings;
        Settings->GetActionMappings(Mappings);
        for (int32 i = Mappings.Num() - 1; i >= 0; --i)
        {
            if (Mappings[i].ActionName == ActionName)
            {
                Settings->RemoveActionMapping(Mappings[i]);
            }
        }
        Settings->AddActionMapping(FInputActionKeyMapping(ActionName, NewKey));
        Settings->SaveKeyMappings();
    }
}

void USettingsWidget::SetMasterVolume(float Volume)
{
    Volume = FMath::Clamp(Volume, 0.f, 1.f);
    if (GEngine)
    {
        if (FAudioDevice* Device = GEngine->GetMainAudioDevice())
        {
            Device->SetTransientMasterVolume(Volume);
        }
    }
}

void USettingsWidget::SetResolution(FIntPoint Resolution, bool bFullscreen)
{
    if (GEngine)
    {
        if (UGameUserSettings* Settings = GEngine->GetGameUserSettings())
        {
            Settings->SetScreenResolution(Resolution);
            Settings->SetFullscreenMode(bFullscreen ? EWindowMode::Fullscreen : EWindowMode::Windowed);
            Settings->ApplySettings(false);
        }
    }
}
