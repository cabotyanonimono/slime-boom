#include "pch.h"
#include "slash_effect_component.h"

#include "engine_time.h"
#include "gui.h"
#include "Rendering/gpu_resource_manager.h"

namespace SlimeBoom
{
void SlashEffectComponent::OnInspectorGui()
{
    engine::Gui::PropertyField("Duration", m_duration_);
    engine::Gui::PropertyField("SlashMeshRenderer", m_slash_mesh_renderer_);
}

void SlashEffectComponent::OnUpdate()
{
    if (!m_is_playing_)
        return;

    if (m_slash_mesh_renderer_ == nullptr)
    {
        m_is_playing_ = false;
        engine::Logger::Warn<SlashEffectComponent>("Slash MeshRenderer is not set");
        return;
    }
    
    m_current_time_ += engine::Time::GetDeltaTime();
    //m_slash_mesh_renderer_->shared_materials[0]->shared_material_block->GetConstantBufferData("Color")->SetFloatData("effect_progress", m_current_time_ / m_duration_);
    
    if (m_current_time_ <= m_duration_)
        return;

    m_slash_mesh_renderer_->GameObject()->SetActive(false);
    m_is_playing_ = false;
}

void SlashEffectComponent::Play()
{
    if (m_slash_mesh_renderer_ == nullptr)
    {
        m_is_playing_ = false;
        engine::Logger::Warn<SlashEffectComponent>("Slash MeshRenderer is not set");
        return;
    }
    
    m_is_playing_ = true;
    m_current_time_ = 0.0f;
    m_slash_mesh_renderer_->GameObject()->SetActive(true);
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::SlashEffectComponent)