#include "Token.h"

UToken::UToken()
{
    TokenType = ETokenType::Power;
    PowerValue = 1.f;
    AreaValue = 1.f;
    Symbol = TEXT("?");
    GlowColor = FLinearColor::White;
}
