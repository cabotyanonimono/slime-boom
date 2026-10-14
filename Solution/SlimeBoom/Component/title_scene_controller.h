#pragma once
#include "scene_transition_controller.h"
#include "../UI/ui_button.h"
#include "Audio/audio_source_component.h"
#include "Components/component.h"

namespace SlimeBoom
{
// タイトルシーンを制御するComponent。プレイボタンでゲームシーンへ遷移する。
class TitleSceneController : public engine::Component
{
    engine::AssetPtr<SceneTransitionController> m_scene_transition_controller_;
    engine::AssetPtr<ui::UIButton> m_play_button_;
    engine::AssetPtr<ui::UIButton> m_stop_button_;

public:
    void OnInspectorGui() override;
    /// プレイボタンに、シーン遷移の実行を登録する。
    void OnStart() override;
    void OnUpdate() override;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_scene_transition_controller_)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_play_button_),
                CEREAL_NVP(m_stop_button_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::TitleSceneController, 2)
