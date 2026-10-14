#pragma once
#include "cancellation_token.h"
#include "iability.h"
#include "../player_data_component.h"
#include "Coroutine/task.h"

// クールダウン制のアビリティの基底クラス。
// クールダウンが経過するたびに、プレイヤーの攻撃回数ぶんExecuteを実行する。再利用はできません。Detachした後は破棄してください。
class CooldownAbility : public IAbility
{
protected:
    float m_cooldown_;
    float m_cooldown_timer_;
    std::shared_ptr<SlimeBoom::PlayerDataComponent> m_player_data_;

    std::shared_ptr<CancellationTokenSource> m_update_attack_cancellation_token_;
    /// プレイヤーの攻撃回数だけ、0.2秒間隔でExecuteを呼ぶコルーチン。
    engine::Task UpdateAttack();
    /// 継承先で実装する、アビリティの発動処理。
    virtual void Execute(float delta_time) = 0;

public:
    /// @param cooldown クールダウン時間(秒)
    /// @param player_data プレイヤーのステータス
    /// @param cooldown_timer クールダウンタイマーの初期値(秒)
    CooldownAbility(float cooldown, const std::shared_ptr<SlimeBoom::PlayerDataComponent> &player_data, float cooldown_timer = 0.0f);
    
    /// クールダウンのタイマーを進め、クールダウンを超えたら攻撃を開始する。
    void Update(float delta_time) override;
    void OnDetach() override;
};
