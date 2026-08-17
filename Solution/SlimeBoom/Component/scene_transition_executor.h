#pragma once
#include "scene_transition_controller.h"
#include "Asset/asset_ptr.h"

namespace SlimeBoom
{
class SceneTransitionExecutor : public engine::Component
{
    engine::AssetPtr<SceneTransitionController> m_scene_transition_controller_;

public:

    void OnInspectorGui() override;
    void OnAwake() override;
    
    template <class Archive>
    void serialize(Archive &ar, const uint32_t version)
    {
        ar(cereal::base_class<engine::Component>(this),
           CEREAL_NVP(m_scene_transition_controller_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::SceneTransitionExecutor, 1)