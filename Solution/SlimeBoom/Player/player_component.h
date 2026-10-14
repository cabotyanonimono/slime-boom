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
#include "game_object.h"

namespace SlimeBoom
{
// プレイヤーの挙動を管理するComponent。
// ステートマシンで待機・移動・回避・死亡を切り替え、被ダメージや回避の処理も行う。
class PlayerComponent final : public engine::Component
{
    static constexpr float kIdleBlendSpeed = 5.0f;
    
    std::shared_ptr<StateMachine> m_state_machine_;

    bool m_is_immune_ = false;
    float m_avoid_cooldown_ = 0.0f;
    float m_avoid_cooldown_timer_ = 0.0f;
    float m_avoid_timer_ = 0.0f;
    float m_dead_delay_timer_ = 0.0f;
    float m_dead_delay_time_ = 0.0f;
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

    /// 入力と回避の状態から、ステートマシンのパラメーター(Move/Avoid/Dead)と無敵状態を更新する。
    void UpdateParameter();
    /// 死亡イベントを発火する。
    void Dead();
    /// ダメージを受ける。無敵中、または回避率による判定で回避した場合は無効になる。HPが0になったら死亡する。
    void TakeDamage(float damage);

public:
    void OnInspectorGui() override;
    /// ステートマシンと状態の遷移条件を構築し、被ダメージとレベルアップのイベントを登録する。
    void OnStart() override;
    void OnUpdate() override;
    void OnFixedUpdate() override;

    /// 死亡してから、死亡演出の待ち時間が経過したかを返す。
    bool IsDeadEffectEnd();
    /// HPが0以下かを返す。
    bool IsDead() const;
    /// 回避を開始したときに呼ばれるコールバックを登録する。
    /// @return RemoveOnAvoidEventに渡すトークン
    size_t AddOnAvoidEvent(const std::function<void()>& callback);
    /// 回避時のコールバックを解除する。
    /// @param token AddOnAvoidEventで受け取ったトークン
    void RemoveOnAvoidEvent(size_t token);
    /// 死亡したときに呼ばれるコールバックを登録する。
    /// @return RemoveOnDeadEventに渡すトークン
    size_t AddOnDeadEvent(const std::function<void()>& callback);
    /// 死亡時のコールバックを解除する。
    /// @param token AddOnDeadEventで受け取ったトークン
    void RemoveOnDeadEvent(size_t token);

    /// 回避のクールダウン時間(秒)を返す。
    float GetAvoidCooldown() const;
    /// 回避のクールダウンの残り時間(秒)を返す。
    float GetAvoidTimer() const;

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

        if (version >= 13)
        {
            ar(
                CEREAL_NVP(m_dead_delay_time_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::PlayerComponent, 13)
