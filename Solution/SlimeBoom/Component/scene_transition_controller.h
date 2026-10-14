#pragma once
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/image.h"
#include "scene.h"

namespace SlimeBoom
{
// 指定したシーンファイルを読み込み、現在のシーンと入れ替えるComponent。
class SceneTransitionController : public engine::Component
{
    std::string m_scene_path_;

    /// シーンファイルを読み込んで新しいシーンを生成し、それまでアクティブだったシーンを破棄する。
    void ChangeScene() const;

public:
    /// 遷移先のシーンファイルのパスを設定する。
    void SetScenePath(const std::string& scene_path);
    /// シーン遷移を実行する。
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
