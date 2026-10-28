#pragma once
#include "ui_button.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/text_renderer.h"

namespace ui
{
// TextRendererの文字列を、UIButtonの中央からオフセットした位置に配置するComponent。
class UIButtonLabel : public engine::Component
{
    engine::AssetPtr<UIButton> m_ui_button_;
    engine::AssetPtr<engine::TextRenderer> m_text_renderer_;
    // ボタンの中心からのずれ。ボタンの大きさに対する割合で、yは下が正。
    Vector2 m_offset_ = Vector2::Zero;

public:
    void OnInspectorGui() override;
    /// ボタンのスクリーン上の中心からオフセットした位置に、文字列の中心が来るように位置と原点を更新する。
    void OnUpdate() override;

    template <class Archive>
    void serialize(Archive &archive, const uint32_t version)
    {
        archive(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_ui_button_),
            CEREAL_NVP(m_text_renderer_)
            );

        if (version >= 2)
        {
            archive(
                CEREAL_NVP(m_offset_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(ui::UIButtonLabel, 2);
