#pragma once
#include "clear_scene_data.h"
#include "scene_transition_controller.h"
#include "scene_transition_executor.h"
#include "../UI/ui_button.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/image.h"
#include "Components/rect_transform.h"
#include "Components/text_renderer.h"
#include "Coroutine/task.h"

namespace SlimeBoom
{
class ClearSceneController : public engine::Component
{
    engine::AssetPtr<engine::Image> m_background_;
    engine::AssetPtr<engine::RectTransform> m_score_board_;
    engine::AssetPtr<engine::TextRenderer> m_game_over_text_;
    engine::AssetPtr<engine::TextRenderer> m_clear_text_;
    engine::AssetPtr<engine::TextRenderer> m_eliminated_slime_count_;
    engine::AssetPtr<engine::TextRenderer> m_level_;
    engine::AssetPtr<engine::GameObject> m_text_ui_;
    engine::AssetPtr<SceneTransitionController> m_restart_scene_transition_controller_;
    engine::AssetPtr<SceneTransitionController> m_quit_scene_transition_controller_;
    engine::AssetPtr<ui::UIButton> m_restart_button_;
    engine::AssetPtr<ui::UIButton> m_quit_button_;

    engine::Task ShowClearSceneBackground() const;
    engine::Task ScoreBoardScaleChangeMotion() const;
    engine::Task ShowClearScene() const;

public:
    void OnInspectorGui() override;
    void OnStart() override;
    void OnEnabled() override;
    void SetClearSceneData(const ClearSceneData& clear_scene_data) const;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            
            CEREAL_NVP(m_eliminated_slime_count_),
            CEREAL_NVP(m_level_)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_background_)
            );
        }

        if (version >= 3)
        {
            ar(
                CEREAL_NVP(m_score_board_)
            );
        }

        if (version >= 4)
        {
            ar(
                CEREAL_NVP(m_game_over_text_),
                CEREAL_NVP(m_clear_text_)
            );
        }

        if (version >= 6)
        {
            ar(
                CEREAL_NVP(m_text_ui_)
            );
        }

        if (version >= 7)
        {
            ar(
                CEREAL_NVP(m_restart_scene_transition_controller_),
                CEREAL_NVP(m_quit_scene_transition_controller_),
                CEREAL_NVP(m_restart_button_),
                CEREAL_NVP(m_quit_button_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::ClearSceneController, 7)