#pragma once
#include "scene_transition_controller.h"
#include "stage_selection.h"
#include "../UI/ui_button.h"
#include "Audio/audio_source_component.h"
#include "Components/component.h"
#include "Components/image.h"

namespace SlimeBoom
{
// タイトルシーンを制御するComponent。プレイボタンでステージ選択メニューを開き、ステージのボタンでゲームシーンへ遷移する。
class TitleSceneController : public engine::Component
{
    engine::AssetPtr<SceneTransitionController> m_scene_transition_controller_;
    engine::AssetPtr<ui::UIButton> m_play_button_;
    engine::AssetPtr<ui::UIButton> m_stop_button_;
    engine::AssetPtr<ui::UIButton> m_stage1_button_;
    engine::AssetPtr<ui::UIButton> m_stage2_button_;
    engine::AssetPtr<ui::UIButton> m_stage3_button_;
    engine::AssetPtr<ui::UIButton> m_back_button_;
    engine::AssetPtr<engine::Image> m_stage_menu_background_;
    engine::AssetPtr<engine::Image> m_title_logo_;

    /// ステージ選択メニューの表示を切り替える。表示中はプレイ・終了ボタンとタイトルロゴを隠し、ステージ・戻るボタンと背景を表示する。
    void ShowStageMenu(bool show) const;
    /// ステージを記録してゲームシーンへ遷移する。
    void SelectStage(kStage stage) const;

public:
    void OnInspectorGui() override;
    /// 各ボタンに、メニューの切り替えやシーン遷移の実行を登録する。
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

        if (version >= 3)
        {
            ar(
                CEREAL_NVP(m_stage1_button_),
                CEREAL_NVP(m_stage2_button_),
                CEREAL_NVP(m_stage3_button_),
                CEREAL_NVP(m_back_button_),
                CEREAL_NVP(m_stage_menu_background_),
                CEREAL_NVP(m_title_logo_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::TitleSceneController, 3)
