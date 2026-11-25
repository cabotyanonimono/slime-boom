#include "pch.h"
#include "slime_simulation_component.h"

#include "gui.h"
#include "../Sound/sound_manager_component.h"
#include "../Utils/gpu_buffer.h"
#include "../Utils/random.h"
#include "Rendering/material.h"

namespace SlimeBoom
{
void SlimeSimulationComponent::SetCurrentSlimesCount(const int count)
{
    m_current_slime_count_ = std::min(count, kMaxSlimesCount);

    const auto group_count = (m_current_slime_count_ + 64 - 1) / 64;

    if (m_slime_generator_compute_ != nullptr)
        m_slime_generator_compute_->SetGroupCountX(group_count);
    if (m_slime_physics_compute_ != nullptr)
        m_slime_physics_compute_->SetGroupCountX(group_count);
    if (m_closest_slime_compute_ != nullptr)
        m_closest_slime_compute_->SetGroupCountX(group_count);
    if (m_player_attack_compute_ != nullptr)
        m_player_attack_compute_->SetGroupCountX(group_count);

    engine::Material::SetGlobalInt("slime_count", m_current_slime_count_);

    SyncRendererInstanceCount();
}

void SlimeSimulationComponent::SyncRendererInstanceCount()
{
    if (m_slime_renderer_ != nullptr)
        m_slime_renderer_->SetInstanceCount(static_cast<uint32_t>(m_current_slime_count_));
}

void SlimeSimulationComponent::UploadSlimeTypes()
{
    if (m_slime_types_.empty())
        m_slime_types_ = SlimeTypes::CreateDefaultDefinitions();
    if (m_slime_types_.size() > SlimeTypes::kMaxCount)
        m_slime_types_.resize(SlimeTypes::kMaxCount);

    std::vector<SlimeTypeParams> params;
    params.reserve(m_slime_types_.size());
    for (const auto &definition : m_slime_types_)
        params.push_back(definition.params);

    m_slime_type_params_buffer_ = GpuBuffer::CreateUploadStructured(
        sizeof(SlimeTypeParams), static_cast<uint32_t>(params.size()), "slime_type_params");
    m_slime_type_params_buffer_.Update(params.data(), sizeof(SlimeTypeParams) * params.size());
    engine::Material::SetGlobalBuffer("slime_type_params", m_slime_type_params_buffer_.GetHandle());
}

void SlimeSimulationComponent::UploadSpawnRates()
{
    if (!m_slime_spawn_rates_buffer_.GetHandle().IsValid())
    {
        m_slime_spawn_rates_buffer_ = GpuBuffer::CreateUploadStructured(
            sizeof(float), SlimeTypes::kMaxCount, "slime_spawn_rates");
        engine::Material::SetGlobalBuffer("slime_spawn_rates", m_slime_spawn_rates_buffer_.GetHandle());
    }
    m_slime_spawn_rates_buffer_.Update(m_spawn_rates_.data(), sizeof(float) * m_spawn_rates_.size());
}

void SlimeSimulationComponent::SetSpawnRate(const std::vector<float> &rates)
{
    m_spawn_rates_.fill(0.0f);
    const auto count = std::min(rates.size(), m_slime_types_.size());
    std::copy_n(rates.begin(), count, m_spawn_rates_.begin());
    UploadSpawnRates();
}

int SlimeSimulationComponent::GetSlimeTypeCount() const
{
    return static_cast<int>(m_slime_types_.size());
}

std::string SlimeSimulationComponent::GetSlimeTypeName(const int type) const
{
    if (type < 0 || type >= GetSlimeTypeCount())
        return "";
    return m_slime_types_[type].name;
}

bool SlimeSimulationComponent::DrawSlimeTypesGui()
{
    if (!ImGui::CollapsingHeader("Slime Types"))
        return false;

    bool changed = false;
    for (int i = 0; i < GetSlimeTypeCount(); ++i)
    {
        ImGui::PushID(i);
        auto &definition = m_slime_types_[i];
        const auto header = std::to_string(i) + ": " + definition.name + "###SlimeType";
        if (ImGui::TreeNode(header.c_str()))
        {
            auto &params = definition.params;
            changed |= engine::Gui::PropertyField("Name", definition.name);
            changed |= engine::Gui::PropertyField("Max HP", params.max_hp);
            changed |= engine::Gui::PropertyField("Speed Scale", params.speed_scale);
            changed |= engine::Gui::PropertyField("Size Scale", params.size_scale);
            changed |= engine::Gui::PropertyField("Texture Index", params.texture_index);
            changed |= engine::Gui::PropertyField("Albedo Tint", params.albedo_tint);
            changed |= engine::Gui::PropertyField("Death Color", params.death_color);
            ImGui::TreePop();
        }
        ImGui::PopID();
    }

    if (GetSlimeTypeCount() < SlimeTypes::kMaxCount && ImGui::Button("Add Slime Type"))
    {
        SlimeTypeDefinition definition;
        definition.name = "New Type";
        m_slime_types_.push_back(definition);
        changed = true;
    }
    if (GetSlimeTypeCount() > 1)
    {
        ImGui::SameLine();
        if (ImGui::Button("Remove Last Slime Type"))
        {
            m_slime_types_.pop_back();
            changed = true;
        }
    }
    return changed;
}

void SlimeSimulationComponent::OnStart()
{
    m_slime_take_damage_event_component_->AddOnDamageEventListener([this](int damage_count){SoundManagerComponent::Play(kSoundTypes::kSlimeTakeDamage);});
    
    m_slime_buffer_ = GpuBuffer::CreateReadWriteStructured(sizeof(SlimeData), kMaxSlimesCount, "slimes");

    engine::Material::SetGlobalBuffer("slimes", m_slime_buffer_.GetHandle());
    engine::Material::SetGlobalInt("slime_count", m_current_slime_count_);
    UploadSlimeTypes();
    UploadSpawnRates();
    SyncRendererInstanceCount();
}

void SlimeSimulationComponent::OnInspectorGui()
{
    engine::Gui::PropertyField("Slime Generator", m_slime_generator_compute_);
    engine::Gui::PropertyField("Slime Physics", m_slime_physics_compute_);
    engine::Gui::PropertyField("Closest Slime", m_closest_slime_compute_);
    engine::Gui::PropertyField("Player Attack", m_player_attack_compute_);
    engine::Gui::PropertyField("Slime Take Damage Event", m_slime_take_damage_event_component_);

    if (DrawSlimeTypesGui())
        UploadSlimeTypes();

    if (engine::Gui::PropertyField("Slime Renderer", m_slime_renderer_))
        SyncRendererInstanceCount();

    if (engine::Gui::PropertyField("Current Slime Count", m_current_slime_count_))
        SetCurrentSlimesCount(m_current_slime_count_);
}

void SlimeSimulationComponent::OnUpdate()
{
    engine::Material::SetGlobalInt("slime_count", m_current_slime_count_);
}

int SlimeSimulationComponent::GetMaxSlimeCount()
{
    return kMaxSlimesCount;
}

int SlimeSimulationComponent::GetCurrentSlimesCount() const
{
    return m_current_slime_count_;
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::SlimeSimulationComponent)