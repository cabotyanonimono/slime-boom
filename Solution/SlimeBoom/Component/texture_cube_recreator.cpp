#include "pch.h"
#include "texture_cube_recreator.h"

#include "gui.h"

namespace SlimeBoom
{
void TextureCubeRecreator::ReCreateTextureCube()
{
    for (const auto& material : m_materials_)
    {
        auto texture_cube = material->shared_material_block->GetTextureCubeData("env_texture");
        if (texture_cube == nullptr)
            texture_cube = material->shared_material_block->GetTextureCubeData("texture_cube");
        texture_cube->is_dirty = true;
    }
}

void TextureCubeRecreator::OnInspectorGui()
{
    for (auto& material : m_materials_)
    {
        ImGui::PushID(material.Lock().get());
        engine::Gui::PropertyField("Material", material);
        ImGui::PopID();
    }

    if (ImGui::Button("Add"))
        m_materials_.emplace_back();
    if (ImGui::Button("Reset"))
        m_materials_.clear();

    if (ImGui::Button("Recreate"))
        ReCreateTextureCube();
}

void TextureCubeRecreator::OnAwake()
{
    ReCreateTextureCube();   
}

void TextureCubeRecreator::OnUpdate()
{
    if (!m_is_first_frame_)
    {
        if (!m_is_executed_)
        {
            ReCreateTextureCube();
            m_is_executed_ = true;
        }
    }

    if (m_is_first_frame_)
        m_is_first_frame_ = false;
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::TextureCubeRecreator)
