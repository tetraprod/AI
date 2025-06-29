#pragma once
#include "CoreMinimal.h"
#include "Token.h"
#include "Engine/DataTable.h"
#include "CompanionToken.generated.h"

UENUM(BlueprintType)
enum class EHellHoundType : uint8
{
    Minor,
    Greater,
    Dire
};

UCLASS(Blueprintable)
class UCompanionToken : public UToken
{
    GENERATED_BODY()
public:
    UCompanionToken();

    /** Which hound class this token unlocks */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Companion")
    TSubclassOf<class AHellHoundCharacter> HoundClass;

    /** Type of the hound for attack scaling */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Companion")
    EHellHoundType HoundType;

    /** Token cost required to purchase this hound */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Companion")
    int32 TokenCost;
};
