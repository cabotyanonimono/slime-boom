#pragma once
#include "Asset/asset_ptr.h"
#include "Components/mesh_renderer.h"

namespace SlimeBoom
{
class SlashEffectComponent : public engine::Component
{
    bool m_is_playing_ = false;
    float m_duration_ = 3.0f;
    float m_current_time_ = 0.0f;
    engine::AssetPtr<engine::MeshRenderer> m_slash_mesh_renderer_;

public:
    void OnInspectorGui() override;
    void OnUpdate() override;

    void Play();
    
    template <class Archive>
void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_duration_),
            CEREAL_NVP(m_slash_mesh_renderer_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::SlashEffectComponent, 1)