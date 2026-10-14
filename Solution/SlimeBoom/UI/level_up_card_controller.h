#pragma once
#include "level_up_card.h"
#include "ui_data_provider.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"

namespace SlimeBoom
{
// レベルアップ画面のカードUIに、現在提示しているカードを設定するComponent。
class LevelUpCardController : public engine::Component
{
    engine::AssetPtr<UiDataProvider> m_ui_data_provider_;
    std::array<engine::AssetPtr<LevelUpCard>, CardController::kCardCount> m_level_up_card_;
    
public:
    void OnInspectorGui() override;
    /// 有効化されたときに、提示しているカードを各カードUIに設定する。
    void OnEnabled() override;
    
    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_level_up_card_),
            CEREAL_NVP(m_ui_data_provider_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::LevelUpCardController, 1)
