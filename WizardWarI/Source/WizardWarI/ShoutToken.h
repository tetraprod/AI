#pragma once
#include "CoreMinimal.h"
#include "Token.h"
#include "ShoutToken.generated.h"

UCLASS(Blueprintable)
class UShoutToken : public UToken
{
    GENERATED_BODY()
public:
    UShoutToken();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Taunt")
    FString DefaultMessage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Taunt")
    int32 TokenCost;
};
