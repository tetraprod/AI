#pragma once
#include "CoreMinimal.h"
#include "Token.h"
#include "LevitationToken.generated.h"

UCLASS(Blueprintable)
class ULevitationToken : public UToken
{
    GENERATED_BODY()
public:
    ULevitationToken();

    /** Additional speed multiplier applied when levitation is active */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Levitation")
    float SpeedMultiplier;
};
