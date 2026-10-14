#pragma once
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "ui_button.h"

namespace ui
{
// AssetPtr<UIButton>を、指しているポインタの値でハッシュするための関数オブジェクト。
struct AssetPtrHash
{
    size_t operator()(
        const engine::AssetPtr<UIButton>& a) const
    {
        return reinterpret_cast<size_t>(a.Lock().get());
    }
};

// キーボードとマウスで、UIボタン間の選択の移動と決定を行うComponent。
// 各ボタンに、上下左右の隣のボタンを設定して使う。
class UINavigator : public engine::Component
{
    using Neighbors = std::unordered_map<kDirection, engine::AssetPtr<UIButton>>;
    std::unordered_map<engine::AssetPtr<UIButton>, Neighbors, AssetPtrHash> m_ui_button_map_;
    engine::AssetPtr<UIButton> m_current_button_;

    /// 決定の入力(Space・Enter・マウスの左ボタン)が押されたかを返す。マウスの左ボタンは、マウスモードがNormalのときのみ有効。
    bool IsPressed();

    /// 選択中のボタンを、指定したボタンに切り替える。
    void SetCurrentPosition(const engine::AssetPtr<UIButton>& ui_button);
    /// 選択中のボタンを、指定した方向の隣のボタンへ移動する。隣のボタンがない場合は何もしない。
    void MovePosition(kDirection dir);

    /// キー入力(WASDまたは矢印キー)で、選択を移動する。
    void UpdatePositionByKey();
    /// マウスのホバーで、選択を移動する。
    void UpdatePositionByMouse();
    /// 決定の入力を、選択中のボタンの押下状態に反映する。
    void UpdateInput();

public:
    void OnInspectorGui() override;
    void OnUpdate() override;

    template <class Archive>
    void serialize(Archive& archive, const uint32_t version)
    {
        archive(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_ui_button_map_),
            CEREAL_NVP(m_current_button_)
        );
    }
};
}

CEREAL_CLASS_VERSION(ui::UINavigator, 1);
