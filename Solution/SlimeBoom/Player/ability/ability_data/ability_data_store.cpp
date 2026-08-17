#include "pch.h"
#include "ability_data_store.h"
#include "gui.h"

namespace SlimeBoom
{
void AbilityDataStore::OnInspectorGui()
{
    for (int i = 0; i < kAbilityTypesCount; ++i)
    {
        ImGui::PushID(i);
        if (ImGui::CollapsingHeader(ToString(static_cast<kAbilityTypes>(i))))
        {
            m_ability_data_[i].OnInspectorGui();
        }
        ImGui::PopID();
    }
}

const AbilityData& AbilityDataStore::GetAbilityData(const kAbilityTypes ability_type)
{
    return m_ability_data_[static_cast<int>(ability_type)];
}

void AbilityDataStore::SetAbilityData(const kAbilityTypes ability_type, const AbilityData& ability_data)
{
    m_ability_data_[static_cast<int>(ability_type)] = ability_data;
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::AbilityDataStore)