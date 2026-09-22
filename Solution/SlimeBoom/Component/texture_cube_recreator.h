#pragma once
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Rendering/material.h"

namespace SlimeBoom
{
class TextureCubeRecreator : public engine::Component
{
    std::vector<engine::AssetPtr<engine::Material>> m_materials_;
    bool m_is_first_frame_ = true;
    bool m_is_executed_ = false;

    void ReCreateTextureCube();
public:
    void OnInspectorGui() override;
    void OnAwake() override;
    void OnUpdate() override;
    
    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_materials_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::TextureCubeRecreator, 2)
