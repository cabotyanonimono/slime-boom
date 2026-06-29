#pragma once
#include "event.h"
#include "input.h"
#include "direction.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/image.h"
#include "Rendering/CabotEngine/Graphics/Texture2D.h"

namespace ui
{
enum class kButtonState
{
    kNormal,
    kSelect,
    kPress
};

class UIButton : public engine::Component
{
    engine::Rect m_rect_ = {Vector2(100, 100), Vector2(100,100)};
    
    kButtonState m_button_state_ = kButtonState::kNormal;
    
    engine::AssetPtr<engine::Image> m_image_;
    engine::AssetPtr<engine::Texture2D> m_normal_texture_;
    engine::AssetPtr<engine::Texture2D> m_selected_texture_;
    engine::AssetPtr<engine::Texture2D> m_pressed_texture_;
    uint32_t m_prev_update_frame_ = 0;
    bool m_is_hovering_ = false;

    engine::Event<> m_events_;
    
    void UpdateMousePosition(Vector2 pos);

public:
    void OnAwake() override;
    void OnInspectorGui() override;
    void OnUpdate() override;

    size_t AddEventListener(const std::function<void()>& callback);
    void RemoveEventListener(size_t token);

    [[nodiscard]] bool IsHovering() const;
    bool IsHoveringStartedThisFrame() const;
    bool IsHoveringEndedThisFrame() const;
    
    engine::Rect GetRect() const;
    void SetIsSelected(bool selected);
    void SetPressed(bool pressed);

    bool IsSelected() const;
    bool IsPressed() const;

    template <class Archive>
    void serialize(Archive &archive, const uint32_t version)
    {
        archive(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_image_),
            CEREAL_NVP(m_normal_texture_),
            CEREAL_NVP(m_selected_texture_),
            CEREAL_NVP(m_pressed_texture_)
            );
    }
};
}

CEREAL_CLASS_VERSION(ui::UIButton, 1);