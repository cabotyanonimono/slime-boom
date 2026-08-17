#pragma once
#include "card_base.h"
#include "../../compute_result.h"
#include "../../Player/player_data_component.h"
#include "../../Player/player_data_presenter.h"
#include "../../Player/player_sword_controller.h"
#include "../../Player/ability/ability_processer.h"
#include "Components/component.h"

namespace SlimeBoom
{
class SlashAbilityCard : public CardBase
{
    engine::AssetPtr<ComputeResult> m_compute_result_;
    engine::AssetPtr<AbilityProcesser> m_ability_processer_;
    engine::AssetPtr<PlayerDataComponent> m_player_data_;
    engine::AssetPtr<PlayerDataPresenter> m_player_data_presenter_;
    engine::AssetPtr<PlayerSwordController> m_sword_controller_;
    engine::AssetPtr<engine::Transform> m_player_transform_;

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
            CEREAL_NVP(m_sword_controller_),
            CEREAL_NVP(m_compute_result_),
            CEREAL_NVP(m_player_transform_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::SlashAbilityCard, 1)
