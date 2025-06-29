#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Localization.h"
#include "LocalizationManager.generated.h"

UCLASS(Blueprintable)
class ULocalizationManager : public UObject
{
    GENERATED_BODY()
public:
    ULocalizationManager();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Localization")
    ELanguage CurrentLanguage;

    UFUNCTION(BlueprintCallable, Category="Localization")
    void SetLanguage(ELanguage NewLanguage);

    UFUNCTION(BlueprintCallable, Category="Localization")
    ELanguage GetLanguage() const { return CurrentLanguage; }

    UFUNCTION(BlueprintCallable, Category="Localization")
    FText GetText(const FString& Key) const;

protected:
    TMap<FString, FText> EnglishTexts;
    TMap<FString, FText> SpanishTexts;
    TMap<FString, FText> FrenchTexts;
    TMap<FString, FText> GermanTexts;
};
