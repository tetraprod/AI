#include "SpellEffectToken.h"

USpellEffectToken::USpellEffectToken()
{
    TokenType = ETokenType::Effect;
    EffectType = ESpellEffectType::Fire;
    Gesture = nullptr;
    Posture = nullptr;
    FacialExpression = nullptr;

    switch (EffectType)
    {
    case ESpellEffectType::Earth:
        Symbol = TEXT("E");
        GlowColor = FLinearColor(0.4f, 0.2f, 0.f);
        break;
    case ESpellEffectType::Air:
        Symbol = TEXT("A");
        GlowColor = FLinearColor(0.8f, 0.8f, 1.f);
        break;
    case ESpellEffectType::Fire:
        Symbol = TEXT("F");
        GlowColor = FLinearColor(1.f, 0.3f, 0.f);
        break;
    case ESpellEffectType::Water:
        Symbol = TEXT("W");
        GlowColor = FLinearColor(0.f, 0.4f, 0.8f);
        break;
    case ESpellEffectType::Electricity:
        Symbol = TEXT("L");
        GlowColor = FLinearColor(1.f, 1.f, 0.f);
        break;
    case ESpellEffectType::Weapon:
        Symbol = TEXT("S");
        GlowColor = FLinearColor(0.7f, 0.7f, 0.7f);
        break;
    case ESpellEffectType::Explosion:
        Symbol = TEXT("X");
        GlowColor = FLinearColor(1.f, 0.5f, 0.2f);
        break;
    case ESpellEffectType::Freeze:
        Symbol = TEXT("I");
        GlowColor = FLinearColor(0.5f, 0.8f, 1.f);
        break;
    default:
        Symbol = TEXT("?");
        GlowColor = FLinearColor::White;
        break;
    }
=======
=======

    FacialExpression = nullptr;
=======

=======
    FacialExpression = nullptr;


}
