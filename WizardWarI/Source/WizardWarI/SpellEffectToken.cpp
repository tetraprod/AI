#include "SpellEffectToken.h"

USpellEffectToken::USpellEffectToken()
{
    TokenType = ETokenType::Effect;
    EffectType = ESpellEffectType::Fire;
    Gesture = nullptr;
    Posture = nullptr;
    FacialExpression = nullptr;
}
