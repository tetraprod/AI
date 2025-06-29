#pragma once
#include "CoreMinimal.h"
#include "Token.h"
#include "ShieldToken.generated.h"

UCLASS(Blueprintable)
class UShieldToken : public UToken
{
    GENERATED_BODY()
public:
    UShieldToken();

    /** Defense multiplier applied when shielding */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shield")
    float DefenseMultiplier;
};
