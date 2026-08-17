#pragma once
#include "card_base.h"
#include "../../Player/player_component.h"
#include "../../Player/ability/ability_processer.h"

namespace SlimeBoom
{
class AuraAbilityCard : public CardBase
{
    engine::AssetPtr<AbilityProcesser> m_ability_processer_;
    engine::AssetPtr<PlayerDataComponent> m_player_data_;
    engine::AssetPtr<PlayerDataPresenter> m_player_data_presenter_;
    engine::AssetPtr<engine::GameObject> m_aura_renderer_;

public:
    void OnInspectorGui() override;
    void OnSelect() override;
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
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::AuraAbilityCard, 1)
