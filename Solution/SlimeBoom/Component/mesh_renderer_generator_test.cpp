#include "pch.h"
#include "mesh_renderer_generator_test.h"

#include "game_object.h"
#include "Asset/Importer/fbx_importer.h"
#include "Rendering/model_importer.h"

void MeshRendererGenerator::OnInspectorGui()
{
    char buf[256];
    strncpy_s(buf, sizeof(buf), m_path_.c_str(), _TRUNCATE);
    buf[sizeof(buf) - 1] = '\0';

    if (ImGui::InputText("text", buf, sizeof(buf), 0))
    {
        m_path_ = buf;
    }
    if (ImGui::Button("Generate"))
    {
        engine::ModelImporter::LoadModelFromFBX(m_path_.c_str());
    }
}