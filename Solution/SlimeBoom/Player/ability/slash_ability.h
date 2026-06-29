#pragma once
#include "cooldown_ability.h"
#include "../player_attack_data.h"
#include "../player_data.h"
#include "../player_data_presenter.h"
#include "../player_sword_controller.h"
#include "../../compute_result.h"
#include "Components/transform.h"

namespace SlimeBoom
{
class SlashAbility : public CooldownAbility
{
    Vector3 m_closest_slime_pos_;
    std::shared_ptr<engine::Transform> m_player_transform_;
    std::shared_ptr<PlayerDataPresenter> m_player_data_presenter_;
    std::shared_ptr<PlayerSwordController> m_sword_controller_;
    std::shared_ptr<ComputeResult> m_compute_result_;
    std::shared_ptr<PlayerData> m_player_data_;
    
    void UpdateCooldown(float delta_time) override;
    
public:
    SlashAbility(const std::shared_ptr<ComputeResult>& compute_result, const std::shared_ptr<PlayerData>& player_data, const std::shared_ptr<engine::Transform>& player_transform, const std::shared_ptr<PlayerDataPresenter>& player_data_presenter, const std::shared_ptr<PlayerSwordController>& swc_controller);
};
}