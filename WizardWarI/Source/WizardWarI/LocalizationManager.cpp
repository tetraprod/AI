#include "LocalizationManager.h"

ULocalizationManager::ULocalizationManager()
{
    CurrentLanguage = ELanguage::English;

    EnglishTexts.Add(TEXT("StartGame"), FText::FromString("Start Game"));
    EnglishTexts.Add(TEXT("Settings"), FText::FromString("Settings"));
    EnglishTexts.Add(TEXT("Controls"), FText::FromString("Controls"));
    EnglishTexts.Add(TEXT("Audio"), FText::FromString("Audio"));
    EnglishTexts.Add(TEXT("Video"), FText::FromString("Video"));

    SpanishTexts.Add(TEXT("StartGame"), FText::FromString("Iniciar juego"));
    SpanishTexts.Add(TEXT("Settings"), FText::FromString("Opciones"));
    SpanishTexts.Add(TEXT("Controls"), FText::FromString("Controles"));
    SpanishTexts.Add(TEXT("Audio"), FText::FromString("Audio"));
    SpanishTexts.Add(TEXT("Video"), FText::FromString("Video"));

    FrenchTexts.Add(TEXT("StartGame"), FText::FromString("D\xC3\xA9marrer le jeu"));
    FrenchTexts.Add(TEXT("Settings"), FText::FromString("Param\xC3\xA8tres"));
    FrenchTexts.Add(TEXT("Controls"), FText::FromString("Commandes"));
    FrenchTexts.Add(TEXT("Audio"), FText::FromString("Audio"));
    FrenchTexts.Add(TEXT("Video"), FText::FromString("Vid\xC3\xA9o"));

    GermanTexts.Add(TEXT("StartGame"), FText::FromString("Spiel starten"));
    GermanTexts.Add(TEXT("Settings"), FText::FromString("Einstellungen"));
    GermanTexts.Add(TEXT("Controls"), FText::FromString("Steuerung"));
    GermanTexts.Add(TEXT("Audio"), FText::FromString("Audio"));
    GermanTexts.Add(TEXT("Video"), FText::FromString("Video"));
}

void ULocalizationManager::SetLanguage(ELanguage NewLanguage)
{
    CurrentLanguage = NewLanguage;
}

FText ULocalizationManager::GetText(const FString& Key) const
{
    switch (CurrentLanguage)
    {
    case ELanguage::Spanish:
        if (const FText* Found = SpanishTexts.Find(Key))
        {
            return *Found;
        }
        break;
    case ELanguage::French:
        if (const FText* Found = FrenchTexts.Find(Key))
        {
            return *Found;
        }
        break;
    case ELanguage::German:
        if (const FText* Found = GermanTexts.Find(Key))
        {
            return *Found;
        }
        break;
    default:
        break;
    }

    if (const FText* Found = EnglishTexts.Find(Key))
    {
        return *Found;
    }

    return FText::FromString(Key);
}
