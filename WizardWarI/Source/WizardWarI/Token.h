#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Token.generated.h"

UENUM(BlueprintType)
enum class ETokenType : uint8
{
    Power,
    Area,
    Effect
};

UCLASS(Blueprintable)
class UToken : public UObject
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Token")
    ETokenType TokenType;


=======


=======
=======




    /** Numerical strength for Power tokens */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Token")
    float PowerValue;

    /** Numerical radius/size for Area tokens */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Token")
    float AreaValue;

=======

=======
=======

=======
=======




};
