#include "pch.h"
#include "level_up_card_controller.h"

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
