#pragma once
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "ui_button.h"

namespace ui
{
struct AssetPtrHash
{
    size_t operator()(
        const engine::AssetPtr<UIButton>& a) const
    {
        return reinterpret_cast<size_t>(a.Lock().get());
    }
};

class UINavigator : public engine::Component
{
    using Neighbors = std::unordered_map<kDirection, engine::AssetPtr<UIButton>>;
    std::unordered_map<engine::AssetPtr<UIButton>, Neighbors, AssetPtrHash> m_ui_button_map_;
    engine::AssetPtr<UIButton> m_current_button_;

    bool IsPressed();

    void SetCurrentPosition(const engine::AssetPtr<UIButton>& ui_button);
    void MovePosition(kDirection dir);

    void UpdatePositionByKey();
    void UpdatePositionByMouse();
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
