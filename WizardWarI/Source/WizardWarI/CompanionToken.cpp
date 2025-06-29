#include "CompanionToken.h"
#include "HellHoundCharacter.h"

UCompanionToken::UCompanionToken()
{
    TokenType = ETokenType::Effect;
    PowerValue = 0.f;
    AreaValue = 0.f;
    HoundType = EHellHoundType::Minor;
    TokenCost = 1;
    Symbol = TEXT("H");
    GlowColor = FLinearColor(1.f, 0.f, 0.f);
}
