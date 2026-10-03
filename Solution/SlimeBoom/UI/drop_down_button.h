#pragma once
#include "ui_button.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/rect_transform.h"

namespace ui
{
class DropDownButton : public engine::Component
{
    bool m_is_open_ = false;
    engine::AssetPtr<UIButton> m_open_button_;
    std::vector<std::pair<engine::AssetPtr<UIButton>, std::shared_ptr<engine::RectTransform>>> m_options_;

    Vector2 m_button_position_;
    Vector2 m_button_size_;

    engine::Event<int> m_on_option_selected_event_;

    void CacheRectTransforms();
    void UpdateButtonsPosition();
    
public:
    void OnInspectorGui() override;
    void OnUpdate() override;

    // これを設定したときUIのButtonも同時に更新されます。
    void SetButtonPosition(Vector2 pos);

    // これを設定したときUIのButtonも同時に更新されます。
    void SetButtonSize(Vector2 size);

    // これを設定したときUIのButtonも同時に更新されます。
    void SetIsOpen(bool is_open);

    void AddOnOptionSelectedEvent(const std::function<void(int)> &func);
    
    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_is_open_),
            CEREAL_NVP(m_button_position_),
            CEREAL_NVP(m_button_size_),
            CEREAL_NVP(m_open_button_),
            CEREAL_NVP(m_options_)
        );
    }
};
}

CEREAL_CLASS_VERSION(ui::DropDownButton, 1)
