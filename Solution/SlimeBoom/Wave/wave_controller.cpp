#include "pch.h"
#include "wave_controller.h"

#include "engine_time.h"
#include "gui.h"

void SlimeBoom::WaveController::OnInspectorGui()
{
    engine::Gui::PropertyField("Slime Simulation", m_slime_simulation_component_);
    
    for (int i = 0; i < m_wave_data_.size(); ++i)
    {
        ImGui::PushID(i);
        auto header = "WaveData" + std::to_string(i);
        if (ImGui::CollapsingHeader(header.c_str()))
        {
            engine::Gui::PropertyField("Time", m_wave_data_[i].time);
            int num_slimes_int = m_wave_data_[i].num_slimes;
            if (engine::Gui::PropertyField("NumSlimes", num_slimes_int))
                m_wave_data_[i].num_slimes = num_slimes_int;
            
            if (ImGui::CollapsingHeader("Spawn Rate"))
            {
                for (int j = 0; j < SlimeTypes::Count; ++j)
                {
                    ImGui::PushID(j);
                    engine::Gui::PropertyField(SlimeTypes::ToString(j).c_str(), m_wave_data_[i].spawn_rates[j]);
                    ImGui::PopID();
                }
            }
        }
        ImGui::PopID();
    }

    if (ImGui::Button("Add"))
        m_wave_data_.emplace_back(WaveData());
    if (ImGui::Button("Remove"))
        m_wave_data_.erase(m_wave_data_.end() - 1);
}

void SlimeBoom::WaveController::OnStart()
{
    m_slime_simulation_component_->SetCurrentSlimesCount(m_wave_data_[m_current_wave_index_].num_slimes);
    m_slime_simulation_component_->SetSpawnRate(m_wave_data_[m_current_wave_index_].spawn_rates);
}

void SlimeBoom::WaveController::OnUpdate()
{
    if (m_current_wave_index_ >= m_wave_data_.size())
        return;
    
    m_current_time_ += engine::Time::GetDeltaTime();
    if (m_current_time_ >= m_wave_data_[m_current_wave_index_].time)
    {
        m_current_time_ -= m_wave_data_[m_current_wave_index_].time;
        ++m_current_wave_index_;
        if (m_current_wave_index_ >= m_wave_data_.size())
        {
            return;
        }

        m_slime_simulation_component_->SetCurrentSlimesCount(m_wave_data_[m_current_wave_index_].num_slimes);
        m_slime_simulation_component_->SetSpawnRate(m_wave_data_[m_current_wave_index_].spawn_rates);
    }
}

uint32_t SlimeBoom::WaveController::GetCurrentNumSlimes() const
{
    return m_wave_data_[m_current_wave_index_].num_slimes;
}

CEREAL_REGISTER_TYPE(SlimeBoom::WaveController)
