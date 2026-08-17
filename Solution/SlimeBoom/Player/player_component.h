#pragma once
#include "player_damage_dispatcher.h"
#include "player_data.h"
#include "player_data_component.h"
#include "player_data_presenter.h"
#include "player_sword_controller.h"
#include "../compute_result.h"
#include "../Component/animator.h"
#include "../Exp/level_up_controller.h"
#include "../Slime/slime_simulation_component.h"
#include "../StateMachine/state_machine.h"
#include "ability/ability_processer.h"
#include "Asset/asset_ptr.h"
#include "Components/camera_component.h"
#include "Components/component.h"
#include "Components/transform.h"
#include "Physics/rigidbody_component.h"

namespace SlimeBoom
{
class PlayerComponent final : public engine::Component
{
    static constexpr float kIdleBlendSpeed = 5.0f;
    
    std::shared_ptr<StateMachine> m_state_machine_;

    bool m_is_immune_ = false;
    float m_avoid_cooldown_ = 0.0f;
    float m_avoid_cooldown_timer_ = 0.0f;
    float m_avoid_timer_ = 0.0f;
    engine::AssetPtr<ComputeResult> m_compute_result_;
    engine::AssetPtr<PlayerDataPresenter> m_player_data_presenter_;
    engine::AssetPtr<PlayerSwordController> m_player_sword_controller_;
    engine::AssetPtr<engine::Transform> m_rotation_transform_;
    engine::AssetPtr<engine::Transform> m_camera_transform_;
    engine::AssetPtr<PlayerDamageDispatcher> m_player_damage_dispatcher_;
    engine::AssetPtr<Animator> m_animator_;
    engine::AssetPtr<engine::GameObject> m_aura_renderer_;
    engine::AssetPtr<engine::Transform> m_player_transform_;
    engine::AssetPtr<PlayerDataComponent> m_player_data_;
    engine::AssetPtr<LevelUpController> m_level_up_controller_;

    engine::Event<> m_on_dead_;
    engine::Event<> m_on_avoid_;

    void UpdateParameter();
    void Dead();
    void TakeDamage(float damage);

public:
    void OnInspectorGui() override;
    void OnStart() override;
    void OnUpdate() override;
    void OnFixedUpdate() override;

    bool IsDead() const;
    size_t AddOnAvoidEvent(const std::function<void()>& callback);
    void RemoveOnAvoidEvent(size_t token);
    size_t AddOnDeadEvent(const std::function<void()>& callback);
    void RemoveOnDeadEvent(size_t token);

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this)
        );

        if (version >= 3)
        {
            ar(
                CEREAL_NVP(m_animator_),
                CEREAL_NVP(m_camera_transform_),
                CEREAL_NVP(m_rotation_transform_)
            );
        }
        
        if (version >= 4)
        {
            ar(
                CEREAL_NVP(m_player_data_presenter_),
                CEREAL_NVP(m_player_sword_controller_)
            );
        }
        
        if (version >= 5)
        {
            ar(
                CEREAL_NVP(m_compute_result_)
            );
        }
        
        if (version >= 6)
        {
            ar(
                CEREAL_NVP(m_player_damage_dispatcher_)
            );
        }

        if (version >= 8)
        {
            ar(
                CEREAL_NVP(m_aura_renderer_)
            );
        }
        
        if (version >= 9)
        {
            ar(
                CEREAL_NVP(m_player_transform_)
            );
        }

        if (version >= 10)
        {
            ar(
                CEREAL_NVP(m_avoid_cooldown_)
            );
        }

        if (version >= 11)
        {
            ar(
                CEREAL_NVP(m_player_data_)
            );
        }

        if (version >= 12)
        {
            ar(
                CEREAL_NVP(m_level_up_controller_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::PlayerComponent, 12)
