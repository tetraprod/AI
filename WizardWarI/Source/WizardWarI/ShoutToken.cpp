#include "ShoutToken.h"

UShoutToken::UShoutToken()
{
    TokenType = ETokenType::Effect;
    PowerValue = 0.f;
    AreaValue = 0.f;
    TokenCost = 1;
    DefaultMessage = TEXT("Roar!");
    Symbol = TEXT("!");
    GlowColor = FLinearColor(1.f, 1.f, 0.f);
}
