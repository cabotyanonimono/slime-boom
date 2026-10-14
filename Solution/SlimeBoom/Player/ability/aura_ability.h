#pragma once
#include "cooldown_ability.h"
#include "../player_data_component.h"
#include "../player_data_presenter.h"
#include "Components/effekseer_renderer.h"
#include "Coroutine/task.h"

namespace SlimeBoom
{
// プレイヤーの周囲全方向を攻撃するオーラアビリティ。クールダウンごとに自動で発動する。
class AuraAbility : public CooldownAbility
{
    std::shared_ptr<PlayerDataPresenter> m_player_data_presenter_;
    std::shared_ptr<PlayerDataComponent> m_player_data_;
    std::shared_ptr<engine::GameObject> m_aura_renderer_;
    std::shared_ptr<engine::EffekseerRenderer> m_effekseer_renderer_;
    
    /// 全方向の攻撃データをPlayerDataPresenterに送り、エフェクトを再生する。次のクールダウンも攻撃速度に応じて更新する。
    void Execute(float delta_time) override;

public:
    /// @param player_data プレイヤーのステータス
    /// @param aura_renderer オーラの表示用GameObject。生成時にアクティブにする。
    /// @param player_data_presenter 攻撃データの送り先
    /// @param effect_renderer 発動時に再生するエフェクト
    AuraAbility( const std::shared_ptr<PlayerDataComponent>& player_data,
                 const std::shared_ptr<engine::GameObject>& aura_renderer,
                 const std::shared_ptr<PlayerDataPresenter>& player_data_presenter,
                 const std::shared_ptr<engine::EffekseerRenderer>& effect_renderer);
};
}
