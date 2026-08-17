#pragma once
#include "cooldown_ability.h"
#include "../player_data.h"
#include "../player_data_component.h"
#include "../player_data_presenter.h"

namespace SlimeBoom
{
class AuraAbility : public CooldownAbility
{
    std::shared_ptr<PlayerDataPresenter> m_player_data_presenter_;
    std::shared_ptr<PlayerDataComponent> m_player_data_;
    std::shared_ptr<engine::GameObject> m_aura_renderer_;

    void UpdateCooldown(float delta_time) override;

public:
    AuraAbility( const std::shared_ptr<PlayerDataComponent>& player_data,
                 const std::shared_ptr<engine::GameObject>& aura_renderer,
                 const std::shared_ptr<PlayerDataPresenter>& player_data_presenter);
};
}
