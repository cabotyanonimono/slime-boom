#pragma once

// アビリティの種類。
enum kAbilityTypes
{
    kAbilityTypesSlash,
    kAbilityTypesAura,

    kAbilityTypesCount
};

/// kAbilityTypesを文字列に変換する。
inline const char* ToString(const kAbilityTypes e)
{
    switch (e)
    {
    case kAbilityTypesSlash: return "kSlash";
    case kAbilityTypesAura: return "kAura";
    default: return "unknown";
    }
}
