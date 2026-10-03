#pragma once

enum class kSoundTypes
{
    kPlayerWalk,
    kPlayerAvoid,
    kAbilitySlash,
    kAbilityAura,
    kSlimeTakeDamage,
    kSlimeEliminate,
    kGetExp,
    kGetExpMany,

    kSoundTypesCount
};

inline const char* ToString(const kSoundTypes e)
{
    switch (e)
    {
    case kSoundTypes::kPlayerWalk: return "kPlayerWalk";
    case kSoundTypes::kPlayerAvoid: return "kPlayerAvoid";
    case kSoundTypes::kAbilitySlash: return "kAbilitySlash";
    case kSoundTypes::kAbilityAura: return "kAbilityAura";
    case kSoundTypes::kSlimeTakeDamage: return "kSlimeTakeDamage";
    case kSoundTypes::kSoundTypesCount: return "kSoundTypesCount";
    case kSoundTypes::kSlimeEliminate: return "kSlimeEliminate";
    case kSoundTypes::kGetExp: return "kGetExp";
    case kSoundTypes::kGetExpMany: return "kGetExpMany";
    default: return "unknown";
    }
}
