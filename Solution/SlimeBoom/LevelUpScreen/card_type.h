#pragma once

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

    kCardType_Count
};

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

    default: return "unknown";
    }
}
