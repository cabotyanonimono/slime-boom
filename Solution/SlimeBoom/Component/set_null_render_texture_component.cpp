#include "pch.h"
#include "set_null_render_texture_component.h"

#include "gui.h"

void SetNullRenderTextureComponent::OnInspectorGui()
{
    engine::Gui::PropertyField("Camera", m_camera_);
    ImGui::Checkbox("Enable", &m_is_enable_);
}

void SetNullRenderTextureComponent::OnUpdate()
{
    if (m_is_first_frame_)
    {
        m_is_first_frame_ = false;
        return;
    }

    if (m_is_enable_)
        m_camera_->SetRenderTexture(engine::AssetPtr<engine::RenderTexture>());
}

CEREAL_REGISTER_TYPE(SetNullRenderTextureComponent)