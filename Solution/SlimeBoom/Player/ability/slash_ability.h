#pragma once
#include "cooldown_ability.h"
#include "../player_attack_data.h"
#include "../player_data.h"
#include "../player_data_component.h"
#include "../player_data_presenter.h"
#include "../player_sword_controller.h"
#include "../../compute_result.h"
#include "Components/effekseer_renderer.h"
#include "Components/transform.h"
#include "ThirdParty/Effekseer/include/EffekseerRendererDX11.h"

namespace SlimeBoom
{
// 最も近いスライムに向かって剣を振るスラッシュアビリティ。クールダウンごとに自動で発動する。
class SlashAbility : public CooldownAbility
{
    Vector3 m_closest_slime_pos_;
    float m_attack_delay_time_;
    std::shared_ptr<engine::Transform> m_player_transform_;
    std::shared_ptr<PlayerDataPresenter> m_player_data_presenter_;
    std::shared_ptr<PlayerSwordController> m_sword_controller_;
    std::shared_ptr<ComputeResult> m_compute_result_;
    std::shared_ptr<PlayerDataComponent> m_player_data_;
    std::shared_ptr<engine::EffekseerRenderer> m_effekseer_renderer_;

    /// 攻撃データを作成し、剣の動きに合わせて遅延させてからPlayerDataPresenterに送るコルーチン。
    engine::Task DelaySetAttackDataTask();
    /// 最も近いスライムの位置を取得して剣を振らせ、攻撃データの送信とエフェクトの再生を行う。
    void Execute(float delta_time) override;

public:
    /// @param compute_result 最も近いスライムの位置を取得するためのComputeResult
    /// @param player_data プレイヤーのステータス
    /// @param player_transform プレイヤーのTransform
    /// @param player_data_presenter 攻撃データの送り先
    /// @param swc_controller 剣を振る動きを制御するPlayerSwordController
    /// @param effekseer_renderer 発動時に再生するエフェクト
    /// @param attack_delay_time 剣を振ってから攻撃判定が出るまでの遅延(秒)
    SlashAbility(const std::shared_ptr<ComputeResult>& compute_result,
                 const std::shared_ptr<PlayerDataComponent>& player_data,
                 const std::shared_ptr<engine::Transform>& player_transform,
                 const std::shared_ptr<PlayerDataPresenter>& player_data_presenter,
                 const std::shared_ptr<PlayerSwordController>& swc_controller,
                 const std::shared_ptr<engine::EffekseerRenderer> &effekseer_renderer,
                 float attack_delay_time);
};
}
