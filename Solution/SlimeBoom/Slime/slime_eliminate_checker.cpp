#include "pch.h"
#include "slime_eliminate_checker.h"

#include "gui.h"

namespace SlimeBoom
{
void SlimeEliminateChecker::OnInspectorGui()
{
    engine::Gui::PropertyField("Compute Result", m_compute_result_);
}

void SlimeEliminateChecker::OnUpdate()
{
    auto current_eliminate_slime_count = *static_cast<int*>(m_compute_result_->GetValue("eliminate_slime_count"));
    auto delta_count = current_eliminate_slime_count - m_eliminate_slime_count_;
    
    if (delta_count > 0)
    {
        m_eliminate_slime_count_ = current_eliminate_slime_count;
        m_on_eliminate_.Invoke(delta_count);
    }
}

size_t SlimeEliminateChecker::AddOnEliminateListener(const std::function<void(int)>& listener)
{
    return m_on_eliminate_.AddListener(listener);
}

void SlimeEliminateChecker::RemoveOnEliminateListener(const size_t token)
{
    m_on_eliminate_.RemoveListener(token);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::SlimeEliminateChecker)