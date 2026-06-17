#pragma once
#include "Asset/asset_ptr.h"
#include "Components/camera_component.h"
#include "Components/component.h"

namespace SlimeBoom
{
class PlayerPositionPresenter : public engine::Component
{
    engine::AssetPtr<engine::Transform> m_player_transform_;

public:
    void OnInspectorGui() override;
    void OnUpdate() override;
    
    template <class Archive>
    void serialize(Archive &ar, const uint32_t version)
    {
        ar(cereal::base_class<Component>(this),
            CEREAL_NVP(m_player_transform_));
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::PlayerPositionPresenter, 1)