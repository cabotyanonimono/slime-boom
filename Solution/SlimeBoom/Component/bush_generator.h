#pragma once
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/mesh_renderer.h"

namespace SlimeBoom
{
class BushGenerator : public engine::Component
{
    bool m_is_dirty_ = false;
    float m_max_distance_;
    uint32_t m_num_bushes_ = 0;
    std::vector<Vector3> m_bush_positions_;
    engine::AssetPtr<engine::MeshRenderer> m_bush_renderer_;
    engine::AssetPtr<engine::Transform> m_player_transform_;

    void UpdateBushPositions();
    void UploadBushPositions() const;
    
public:
    void OnInspectorGui() override;
    void OnStart() override;
    void OnUpdate() override;
    
    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_max_distance_),
            CEREAL_NVP(m_num_bushes_),
            CEREAL_NVP(m_bush_renderer_),
            CEREAL_NVP(m_player_transform_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::BushGenerator, 1)