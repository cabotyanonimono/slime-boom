#pragma once
#include "card_base.h"
#include <Effekseer.h>
#include "../../Player/player_component.h"
#include "../../Player/ability/ability_processer.h"
#include "Components/effekseer_renderer.h"

namespace SlimeBoom
{
// オーラ(周囲全方向への攻撃)アビリティを習得するカード。
class AuraAbilityCard : public CardBase
{
    engine::AssetPtr<AbilityProcesser> m_ability_processer_;
    engine::AssetPtr<PlayerDataComponent> m_player_data_;
    engine::AssetPtr<PlayerDataPresenter> m_player_data_presenter_;
    engine::AssetPtr<engine::GameObject> m_aura_renderer_;
    engine::AssetPtr<engine::EffekseerRenderer> m_effekseer_renderer_;

public:
    void OnInspectorGui() override;
    /// AuraAbilityを生成して、AbilityProcesserに追加する。
    void OnSelect() override;
    /// オーラアビリティを未習得の場合のみ選択可能。
    bool CanSelect() override;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<CardBase>(this),
            CEREAL_NVP(m_ability_processer_),
            CEREAL_NVP(m_player_data_),
            CEREAL_NVP(m_player_data_presenter_),
            CEREAL_NVP(m_aura_renderer_)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_effekseer_renderer_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::AuraAbilityCard, 2)