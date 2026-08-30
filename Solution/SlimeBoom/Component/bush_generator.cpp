#include "pch.h"
#include "bush_generator.h"
#include <utility>
#include "../Utils/random.h"
#include "Components/renderer_2d.h"

namespace SlimeBoom
{
void BushGenerator::UpdateBushPositions()
{
    m_bush_positions_.reserve(m_num_bushes_);
    for (int i = 0; std::cmp_less(i, m_num_bushes_); ++i)
    {
        if ((m_player_transform_->Position() - m_bush_positions_[i]).Length() > m_max_distance_)
        {
            m_is_dirty_ = true;
            auto random_pos = Vector3(Random(-m_max_distance_, m_max_distance_), 0, Random(-m_max_distance_, m_max_distance_));
            m_bush_positions_[i] = random_pos + m_player_transform_->Position();
        }
    }
}

void BushGenerator::UploadBushPositions()
{
    if (!m_is_dirty_)
        return;
    
    const auto positions_buffer = m_bush_renderer_->shared_materials[0]->shared_material_block->GetStructuredBufferData("positions");
    const auto shadow_positions_buffer = m_bush_renderer_->GetShadowMaterial()->shared_material_block->GetStructuredBufferData("positions");

    if (positions_buffer == nullptr)
        return;

    if (positions_buffer->Stride() != sizeof(Vector3))
    {
        positions_buffer->SetStride(sizeof(Vector3));
        shadow_positions_buffer->SetStride(sizeof(Vector3));
    }
    
    if (positions_buffer->Count() <= m_bush_positions_.size())
    {
        positions_buffer->SetCount(m_bush_positions_.size());
        shadow_positions_buffer->SetCount(m_bush_positions_.size());
    }
        
    positions_buffer->SetData(m_bush_positions_.data());
    shadow_positions_buffer->SetData(m_bush_positions_.data());
    m_is_dirty_ = false;
}

void BushGenerator::OnInspectorGui()
{
    engine::Gui::PropertyField("Num Bushes", m_num_bushes_);
    engine::Gui::PropertyField("Max Distance", m_max_distance_);
    engine::Gui::PropertyField("Bush Renderer", m_bush_renderer_);
    engine::Gui::PropertyField("Player Transform", m_player_transform_);
}

void BushGenerator::OnStart()
{
    m_bush_positions_.reserve(m_num_bushes_);
    for (int i = 0; std::cmp_less(i, m_num_bushes_); ++i)
    {
        auto random_pos = Vector3(Random(-m_max_distance_, m_max_distance_), 0, Random(-m_max_distance_, m_max_distance_));
        m_bush_positions_.emplace_back(random_pos + m_player_transform_->Position());
        m_is_dirty_ = true;
    }
}

void BushGenerator::OnUpdate()
{
    UpdateBushPositions();
    UploadBushPositions();
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::BushGenerator)