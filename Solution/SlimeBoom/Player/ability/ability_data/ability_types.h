#pragma once

enum kAbilityTypes
{
    kAbilityTypesSlash,
    kAbilityTypesAura,

    kAbilityTypesCount
};

inline const char* ToString(const kAbilityTypes e)
{
    switch (e)
    {
    case kAbilityTypesSlash: return "kSlash";
    case kAbilityTypesAura: return "kAura";
    default: return "unknown";
    }
}
