#include "pch.h"
#include "play_anim_state.h"

namespace SlimeBoom
{
void PlayAnimState::OnEnter()
{
    auto current_anim_name = m_animator_->GetCurrentAnimName();
    if (current_anim_name != "")
    {
        m_animator_->BlendAnim(current_anim_name, m_anim_name_, m_blend_speed_);
        return;
    }

    m_animator_->PlayAnim(m_anim_name_);
}

void PlayAnimState::Update()
{
}

PlayAnimState::PlayAnimState(const std::shared_ptr<Animator>& animator, const std::string& anim_name, const float blend_speed)
    : m_animator_(animator), m_anim_name_(anim_name), m_blend_speed_(blend_speed)
{
}
}
