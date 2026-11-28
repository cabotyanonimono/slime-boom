#pragma once
#include "level_up_card.h"
#include "level_up_status_view.h"
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
    // カードの左右に、持っているスキルとプレイヤーのステータスを出すRenderer。シーンに置かず、OnStartで追加する
    std::shared_ptr<LevelUpStatusView> m_status_view_;

    /// 1フレーム待ってから、スキルとステータスの一覧を出すRendererを自分のGameObjectに追加する。
    engine::Task AddStatusView();
    
public:
    void OnInspectorGui() override;
    /// スキルとステータスの一覧を出すRendererの追加を始める。
    void OnStart() override;
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
