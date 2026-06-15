#pragma once
#include "Components/component.h"

class MeshRendererGenerator : public engine::Component
{
    std::string m_path_;
public:
    void OnInspectorGui() override;
};
