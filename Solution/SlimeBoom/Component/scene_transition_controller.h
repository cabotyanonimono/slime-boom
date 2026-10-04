#pragma once
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/image.h"
#include "scene.h"

namespace SlimeBoom
{
class SceneTransitionController : public engine::Component
{
    std::string m_scene_path_;

    void ChangeScene() const;

public:
    void SetScenePath(const std::string& scene_path);
    void Execute() const;
    void OnInspectorGui() override;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_scene_path_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::SceneTransitionController, 1)
