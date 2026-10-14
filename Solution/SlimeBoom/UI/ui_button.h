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
// ボタンの状態(通常・選択中・押下中)。
enum class kButtonState
{
    kNormal,
    kSelect,
    kPress
};

// 状態に応じて画像を切り替えるUIボタン。マウスのホバーによる選択の検出と、押されたときのイベントの通知を行う。
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
    
    /// マウスの位置から、ホバー状態とホバーが切り替わったフレームを更新する。
    void UpdateMousePosition(Vector2 pos);

public:
    void OnStart() override;
    void OnInspectorGui() override;
    /// マウスの位置を更新し、画像の矩形からスクリーン座標でのボタンの矩形を計算する。
    void OnUpdate() override;

    /// ボタンが押されて離されたときに呼ばれるコールバックを登録する。
    /// @return RemoveEventListenerに渡すトークン
    size_t AddEventListener(const std::function<void()>& callback);
    /// コールバックを解除する。
    /// @param token AddEventListenerで受け取ったトークン
    void RemoveEventListener(size_t token);

    /// マウスがボタン上にあるかを返す。
    [[nodiscard]] bool IsHovering() const;
    /// このフレームでマウスがボタン上に入ったかを返す。
    bool IsHoveringStartedThisFrame() const;
    /// このフレームでマウスがボタン上から出たかを返す。
    bool IsHoveringEndedThisFrame() const;
    
    /// スクリーン座標でのボタンの矩形を返す。
    engine::Rect GetRect() const;
    /// 選択状態を設定し、状態に合わせた画像に切り替える。
    void SetIsSelected(bool selected);
    /// 押下状態を設定する。選択中に押すと押下状態になり、押下中に離すとイベントを発火して選択状態に戻る。
    void SetPressed(bool pressed);

    /// 選択中かを返す。
    bool IsSelected() const;
    /// 押下中かを返す。
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