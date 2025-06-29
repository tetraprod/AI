#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CastleHub.generated.h"

UCLASS()
class ACastleHub : public AActor
{
    GENERATED_BODY()
public:
    ACastleHub();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* TokenChest;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* RobeDisplay;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* SpellBookShelf;
};
