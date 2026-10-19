#pragma once
#include "scene_transition_controller.h"
#include "../UI/ui_button.h"
#include "Components/component.h"

namespace SlimeBoom
{
// 難易度(ステージ)。
enum class kStage
{
    kStage1 = 1,
    kStage2 = 2,
    kStage3 = 3
};

// ステージ選択画面で選ばれたステージを、シーンをまたいで保持する。
class StageSelection
{
    inline static kStage m_stage_ = kStage::kStage1;

public:
    /// 選択したステージを設定する。
    static void SetStage(kStage stage) { m_stage_ = stage; }
    /// 選択されているステージを返す。
    static kStage GetStage() { return m_stage_; }
};

// ステージ選択シーンを制御するComponent。ステージのボタンでステージを記録し、ゲームシーンへ遷移する。
class StageSelectSceneController : public engine::Component
{
    engine::AssetPtr<SceneTransitionController> m_scene_transition_controller_;
    engine::AssetPtr<ui::UIButton> m_stage1_button_;
    engine::AssetPtr<ui::UIButton> m_stage2_button_;
    engine::AssetPtr<ui::UIButton> m_stage3_button_;

    /// ステージを記録してゲームシーンへ遷移する。
    void SelectStage(kStage stage) const;

public:
    void OnInspectorGui() override;
    /// 各ステージのボタンに、ステージの記録とシーン遷移の実行を登録する。
    void OnStart() override;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_scene_transition_controller_),
            CEREAL_NVP(m_stage1_button_),
            CEREAL_NVP(m_stage2_button_)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_stage3_button_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::StageSelectSceneController, 2)
