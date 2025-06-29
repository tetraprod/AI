#pragma once
#include "CoreMinimal.h"
#include "Token.h"
#include "AreaToken.generated.h"

UENUM(BlueprintType)
enum class EAreaTarget : uint8
{
    Direct,
    Wide,
    Radiating,
    Self
};

UCLASS(Blueprintable)
class UAreaToken : public UToken
{
    GENERATED_BODY()
public:
    UAreaToken();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Area")
    EAreaTarget AreaTarget;
};
