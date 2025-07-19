#include "AreaToken.h"

UAreaToken::UAreaToken()
{
    TokenType = ETokenType::Area;
    AreaTarget = EAreaTarget::Direct;
    Symbol = TEXT("A");
    GlowColor = FLinearColor(0.f, 1.f, 0.f);
}
