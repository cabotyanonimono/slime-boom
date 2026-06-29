#pragma once
#include "slash_ability_data.h"
#include "Components/component.h"

namespace SlimeBoom
{
class AbilityData : public engine::Component
{
    inline static SlashAbilityData m_slash_ability_data_;

public:
    void OnInspectorGui() override;

    static SlashAbilityData GetSlashAbilityData();
    static void SetSlashAbilityData(const SlashAbilityData& ability_data);

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_slash_ability_data_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::AbilityData, 2)
