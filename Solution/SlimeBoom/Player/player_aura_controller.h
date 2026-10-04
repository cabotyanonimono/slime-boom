#pragma once
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/mesh_renderer.h"

namespace SlimeBoom
{
class PlayerAuraController : public engine::Component
{
    bool m_is_fading_in_ = true;
    float m_alpha_ = 0.0f;
    float m_alpha_max_ = 1.0f;
    float m_alpha_min_ = 0.0f;
    engine::AssetPtr<engine::MeshRenderer> m_aura_renderer_;

public:
    void OnInspectorGui() override;
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
                CEREAL_NVP(m_aura_renderer_)
            );
        }

        if (version >= 3)
        {
            ar(
                CEREAL_NVP(m_alpha_max_),
                CEREAL_NVP(m_alpha_min_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::PlayerAuraController, 3)
