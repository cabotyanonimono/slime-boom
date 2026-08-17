#pragma once
#include "../../Component/animator.h"
#include "../../StateMachine/state.h"

namespace SlimeBoom
{
class PlayAnimState : public State
{
    std::shared_ptr<Animator> m_animator_;
    std::string m_anim_name_;
    float m_blend_speed_;

protected:
    void OnEnter() override;
    void Update() override;
    
public:
    PlayAnimState(const std::shared_ptr<Animator>& animator, const std::string& anim_name, float blend_speed = 1.0f);
    
};
}