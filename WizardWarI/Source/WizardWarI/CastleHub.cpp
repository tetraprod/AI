#include "CastleHub.h"
#include "Components/StaticMeshComponent.h"

ACastleHub::ACastleHub()
{
    TokenChest = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TokenChest"));
    RootComponent = TokenChest;

    RobeDisplay = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RobeDisplay"));
    RobeDisplay->SetupAttachment(RootComponent);

    SpellBookShelf = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpellBookShelf"));
    SpellBookShelf->SetupAttachment(RootComponent);
}
