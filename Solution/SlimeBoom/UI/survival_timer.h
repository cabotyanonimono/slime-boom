#pragma once
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/text_renderer.h"

namespace SlimeBoom
{
class SurvivalTimer : public engine::Component
{
    static constexpr float kSecondsPerMinute = 60.0f;

    uint32_t m_minutes_ = 0;
    float m_seconds_ = 0.0f;

    engine::AssetPtr<engine::TextRenderer> m_text_renderer_;

public:
    void OnInspectorGui() override;
    void OnUpdate() override;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_text_renderer_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::SurvivalTimer, 1)
