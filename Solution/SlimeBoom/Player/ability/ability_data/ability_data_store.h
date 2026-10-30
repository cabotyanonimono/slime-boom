#pragma once
#include <propsys.h>

#include "ability_data.h"
#include "ability_types.h"
#include "aura_ability_data.h"
#include "slash_ability_data.h"
#include "Components/component.h"

namespace SlimeBoom
{
// アビリティの種類ごとのAbilityDataを、staticに保持するComponent。どこからでも参照できる。
class AbilityDataStore : public engine::Component
{
    inline static std::array<AbilityData, kAbilityTypesCount> m_ability_data_;

public:
    void OnInspectorGui() override;

    /// 指定した種類のアビリティのデータを返す。
    static const AbilityData& GetAbilityData(kAbilityTypes ability_type);
    /// 指定した種類のアビリティのデータを設定する。
    static void SetAbilityData(kAbilityTypes ability_type, const AbilityData& ability_data);

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this)
        );

        if (version == 3)
        {
            // バージョン3はSlashとAuraの2種類だけを保存しているので、それぞれの位置に読み込む
            std::array<AbilityData, 2> old_ability_data;
            ar(
                cereal::make_nvp("m_ability_data_", old_ability_data)
            );
            std::copy(old_ability_data.begin(), old_ability_data.end(), m_ability_data_.begin());
        }

        if (version >= 4)
        {
            ar(
                CEREAL_NVP(m_ability_data_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::AbilityDataStore, 4)
