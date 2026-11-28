#include "pch.h"
#include "level_up_card_controller.h"

#include "engine.h"
#include "gui.h"

namespace SlimeBoom
{
void LevelUpCardController::OnInspectorGui()
{
    engine::Gui::PropertyField("UI Data Provider", m_ui_data_provider_);

    for (int i = 0; i < m_level_up_card_.size(); ++i)
    {
        ImGui::PushID(i);
        engine::Gui::PropertyField("Level Up Card", m_level_up_card_[i]);
        ImGui::PopID();
    }
}

engine::Task LevelUpCardController::AddStatusView()
{
    // OnStartはGameObjectがComponentの一覧を回している途中で呼ばれるので、そこでAddComponentすると一覧が壊れる。
    // 1フレーム待って、一覧を回していないときに追加する
    // 待っている間に破棄されることがあるので、thisではなく弱参照で持っておき、再開後に生きているか確かめる
    const std::weak_ptr weak_self = shared_from_base<LevelUpCardController>();
    co_await engine::WaitForNextFrame();

    const auto self = weak_self.lock();
    if (self == nullptr || self->IsDestroying())
        co_return;
    const auto game_object = self->GameObject();
    if (game_object == nullptr || game_object->IsDestroying())
        co_return;

    self->m_status_view_ = game_object->AddComponent<LevelUpStatusView>();
    self->m_status_view_->Setup(self->m_ui_data_provider_);
}

void LevelUpCardController::OnStart()
{
    engine::Engine::coroutine.Start(AddStatusView());
}

void LevelUpCardController::OnEnabled()
{
    const auto cards = m_ui_data_provider_->GetCards();
    for (int i = 0; i < m_level_up_card_.size(); ++i)
    {
        if (m_level_up_card_[i] == nullptr)
            continue;

        m_level_up_card_[i]->SetCard(cards[i]);
    }
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::LevelUpCardController)
