#pragma once
#include "../Component/scene_transition_controller.h"
#include "../UI/text_color_changer.h"
#include "../UI/ui_button.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Coroutine/task.h"
#include "game_object.h"

namespace SlimeBoom
{
// ポーズ画面の開始と終了を制御するComponent。
// Escapeキーで、TimeScaleを0にしてゲームを止め、マウスを解放してポーズUIを表示する。
// 再開ボタンまたはEscapeキーで再開し、やめるボタンでシーン遷移する。
// ポーズ中は、暗転の影響を受けないHUDのテキストの色も、TextColorChangerで暗くする。
class PauseScreenManager final : public engine::Component
{
    engine::AssetPtr<engine::GameObject> m_pause_ui_object_;
    engine::AssetPtr<ui::UIButton> m_resume_button_;
    engine::AssetPtr<ui::UIButton> m_quit_button_;
    engine::AssetPtr<SceneTransitionController> m_quit_scene_transition_controller_;
    // HUDのテキストの色を、ポーズ中だけ変えるComponent。未設定でも動作する。
    engine::AssetPtr<TextColorChanger> m_text_color_changer_;
    bool m_is_paused_ = false;
    bool m_is_resuming_ = false;

    /// マウスを解放し、TimeScaleを0にしてポーズUIを表示する。HUDのテキストの色も暗くする。
    void Pause();
    /// 1フレーム待ってからマウスをロックし、TimeScaleを戻してポーズUIを閉じる。HUDのテキストの色も元に戻す。
    /// 待つのは、再開ボタンのクリックがプレイヤーの操作に伝わらないようにするため。
    engine::Task Resume();

public:
    void OnInspectorGui() override;
    /// 再開ボタンとやめるボタンに、それぞれの処理を登録する。
    void OnStart() override;
    /// Escapeキーでポーズの開始と終了を切り替える。
    /// レベルアップ画面やクリア画面などでゲームが停止中のときは、ポーズを開始しない。
    void OnUpdate() override;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_pause_ui_object_),
            CEREAL_NVP(m_resume_button_),
            CEREAL_NVP(m_quit_button_),
            CEREAL_NVP(m_quit_scene_transition_controller_)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_text_color_changer_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::PauseScreenManager, 2)
