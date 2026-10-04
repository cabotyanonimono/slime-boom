#pragma once
#include "cooldown_ability.h"
#include "../player_data_component.h"
#include "../player_data_presenter.h"
#include "Components/effekseer_renderer.h"
#include "Coroutine/task.h"

namespace SlimeBoom
{
class AuraAbility : public CooldownAbility
{
    std::shared_ptr<PlayerDataPresenter> m_player_data_presenter_;
    std::shared_ptr<PlayerDataComponent> m_player_data_;
    std::shared_ptr<engine::GameObject> m_aura_renderer_;
    std::shared_ptr<engine::EffekseerRenderer> m_effekseer_renderer_;
    
    void Execute(float delta_time) override;

public:
    AuraAbility( const std::shared_ptr<PlayerDataComponent>& player_data,
                 const std::shared_ptr<engine::GameObject>& aura_renderer,
                 const std::shared_ptr<PlayerDataPresenter>& player_data_presenter,
                 const std::shared_ptr<engine::EffekseerRenderer>& effect_renderer);
};
}
