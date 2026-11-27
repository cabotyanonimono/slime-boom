#pragma once

// レベルアップカードの種類。
enum kCardType
{
    kCardType_MaxHp,
    kCardType_Speed,
    kCardType_AttackPower,
    kCardType_AttackSpeed,
    kCardType_ExpMultiplier,
    kCardType_AuraAbility,
    kCardType_Slash,
    kCardType_AttackRange,
    kCardType_AttackNum,
    kCardType_AvoidPercent,
    kCardType_AcquisitionRange,
    kCardType_ExplosionAbility,
    kCardType_SlashAttackPower,
    kCardType_AuraAttackPower,
    kCardType_ExplosionAttackPower,
    kCardType_SlashAttackRange,
    kCardType_AuraAttackRange,
    kCardType_ExplosionAttackRange,
    kCardType_SlashAttackSpeed,
    kCardType_AuraAttackSpeed,

    kCardType_Count
};

/// kCardTypeを文字列に変換する。
inline const char* ToString(const kCardType e)
{
    switch (e)
    {
    case kCardType_MaxHp: return "kCardType_MaxHp";
    case kCardType_Speed: return "kCardType_Speed";
    case kCardType_AttackPower: return "kCardType_AttackPower";
    case kCardType_AttackSpeed: return "kCardType_AttackSpeed";
    case kCardType_ExpMultiplier: return "kCardType_ExpMultiplier";
    case kCardType_AuraAbility: return "kCardType_AuraAbility";
    case kCardType_Slash: return "kCardType_Slash";
    case kCardType_AttackRange: return "kCardType_AttackRange";
    case kCardType_AttackNum: return "kCardType_AttackNum";
    case kCardType_AvoidPercent: return "kCardType_AvoidPercent";
    case kCardType_AcquisitionRange: return "kCardType_AcquisitionRange";
    case kCardType_ExplosionAbility: return "kCardType_ExplosionAbility";
    case kCardType_SlashAttackPower: return "kCardType_SlashAttackPower";
    case kCardType_AuraAttackPower: return "kCardType_AuraAttackPower";
    case kCardType_ExplosionAttackPower: return "kCardType_ExplosionAttackPower";
    case kCardType_SlashAttackRange: return "kCardType_SlashAttackRange";
    case kCardType_AuraAttackRange: return "kCardType_AuraAttackRange";
    case kCardType_ExplosionAttackRange: return "kCardType_ExplosionAttackRange";
    case kCardType_SlashAttackSpeed: return "kCardType_SlashAttackSpeed";
    case kCardType_AuraAttackSpeed: return "kCardType_AuraAttackSpeed";

    default: return "unknown";
    }
}
