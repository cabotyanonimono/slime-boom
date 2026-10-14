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
#include "game_object.h"

namespace SlimeBoom
{
// クリア/ゲームオーバー画面を制御するComponent。
// 画面の演出と、リスタート・終了ボタンによるシーン遷移を担当する。
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

    /// 背景を徐々に不透明にしながら表示するコルーチン。
    engine::Task ShowClearSceneBackground() const;
    /// スコアボードを少し大きく拡大してから元の大きさに戻しつつ表示するコルーチン。
    engine::Task ScoreBoardScaleChangeMotion() const;
    /// 背景の表示、スコアボードの演出の順に実行するコルーチン。
    engine::Task ShowClearScene() const;

public:
    void OnInspectorGui() override;
    void OnStart() override;
    /// 有効化されたときにクリア画面の演出を開始する。
    void OnEnabled() override;

    /// 変更したマテリアルの状態を初期化する
    void OnDestroy() override;
    /// 撃破数とレベルを表示し、クリア/ゲームオーバーのどちらの文言を出すか切り替える。
    /// @param clear_scene_data 画面に表示する結果データ
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