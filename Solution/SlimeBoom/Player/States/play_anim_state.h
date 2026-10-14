#pragma once
#include "../../Component/animator.h"
#include "../../StateMachine/state.h"

namespace SlimeBoom
{
// 指定したアニメーションを再生するだけの状態。
class PlayAnimState : public State
{
    std::shared_ptr<Animator> m_animator_;
    std::string m_anim_name_;
    float m_blend_speed_;

protected:
    /// 現在のアニメーションから指定のアニメーションへブレンドして切り替える。
    void OnEnter() override;
    void Update() override;
    
public:
    /// @param animator アニメーションの制御に使うAnimator
    /// @param anim_name 再生するアニメーション名
    /// @param blend_speed 切り替え時のブレンド速度
    PlayAnimState(const std::shared_ptr<Animator>& animator, const std::string& anim_name, float blend_speed = 1.0f);
    
};
}