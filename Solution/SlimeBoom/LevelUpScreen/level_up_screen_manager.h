#pragma once
#include "card_controller.h"
#include "../UI/ui_button.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/text_renderer.h"

namespace SlimeBoom
{
// レベルアップ画面の開始と終了を制御するComponent。
// 画面の表示中はTimeScaleを0にしてゲームを止め、マウスを解放する。
class LevelUpScreenManager final : public engine::Component
{
    engine::AssetPtr<CardController> m_card_controller_;
    engine::AssetPtr<LevelUpController> m_level_up_controller_;
    engine::AssetPtr<engine::GameObject> m_level_up_ui_object_;
    engine::AssetPtr<engine::TextRenderer> m_timer_text_;
    engine::AssetPtr<engine::TextRenderer> m_hp_text_;
    engine::AssetPtr<engine::TextRenderer> m_level_text_;
    std::array<engine::AssetPtr<ui::UIButton>, CardController::kCardCount> m_buttons_;
    
    /// マウスを解放してカードを抽選し、TimeScaleを0にしてレベルアップのUIを表示する。HUDのテキスト色も画面用に変更する。
    void ScreenStart() const;
    /// 1フレーム待ってからマウスをロックし、TimeScaleを戻してUIを閉じる。HUDのテキスト色も元に戻す。
    engine::Task ScreenEnd() const;
    
public:
    void OnInspectorGui() override;
    /// レベルアップの通知で画面を開始し、いずれかのカードボタンが押されたら画面を終了するよう登録する。
    void OnStart() override;
    
    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_card_controller_),
            CEREAL_NVP(m_level_up_controller_),
            CEREAL_NVP(m_level_up_ui_object_)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_buttons_)
            );
        }

        if (version >= 3)
        {
            ar(
                CEREAL_NVP(m_timer_text_),
                CEREAL_NVP(m_hp_text_),
                CEREAL_NVP(m_level_text_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::LevelUpScreenManager, 3)
