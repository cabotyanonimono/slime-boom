#pragma once

namespace SlimeBoom
{
// アビリティの汎用パラメーター(効果量・数・角度・サイズ・持続時間・クールダウン・ノックバック)。
struct AbilityData : engine::Inspectable
{
    float value;
    float count;
    float arc;
    float size;
    float duration;
    float cooldown;
    float knockback;

    void OnInspectorGui() override;
    
    template<typename Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            CEREAL_NVP(value),
            CEREAL_NVP(count),
            CEREAL_NVP(arc),
            CEREAL_NVP(size),
            CEREAL_NVP(duration),
            CEREAL_NVP(cooldown),
            CEREAL_NVP(knockback)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::AbilityData, 1)