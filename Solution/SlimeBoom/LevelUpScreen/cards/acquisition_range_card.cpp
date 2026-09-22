#include "pch.h"
#include "acquisition_range_card.h"

#include "gui.h"

void SlimeBoom::AcquisitionRangeCard::OnInspectorGui()
{
    CardBase::OnInspectorGui();
    engine::Gui::PropertyField("Value", m_value_);
    engine::Gui::PropertyField("Exp Simulation", m_exp_simulation_);
}

void SlimeBoom::AcquisitionRangeCard::OnSelect()
{
    auto acquisition_range = m_exp_simulation_->GetAcquisitionRange();
    acquisition_range += m_value_;
    m_exp_simulation_->SetAcquisitionRange(acquisition_range);
}

CEREAL_REGISTER_TYPE(SlimeBoom::AcquisitionRangeCard)
