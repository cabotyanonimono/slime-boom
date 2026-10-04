#include "pch.h"
#include "player_aura_controller.h"

#include "engine_time.h"
#include "gui.h"

namespace SlimeBoom
{
void PlayerAuraController::OnInspectorGui()
{
    engine::Gui::PropertyField("Aura Renderer", m_aura_renderer_);
    engine::Gui::PropertyField("Alpha Max", m_alpha_max_);
    engine::Gui::PropertyField("Alpha Min", m_alpha_min_);
}

void PlayerAuraController::OnUpdate()
{
    m_alpha_ += (m_is_fading_in_ ? 0.1f : -0.1f) * engine::Time::GetDeltaTime();
    if (m_alpha_ > m_alpha_max_)
        m_is_fading_in_ = false;
    if (m_alpha_ < m_alpha_min_)
        m_is_fading_in_ = true;

    auto buff_data = m_aura_renderer_->shared_materials[0]->shared_material_block->GetConstantBufferData("Alpha");
    buff_data->SetFloatData("alpha", m_alpha_);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::PlayerAuraController)