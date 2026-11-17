#include "pch.h"
#include "bush_generator.h"
#include <cfloat>
#include <utility>

#include "gui.h"
#include "../Utils/random.h"
#include "Components/renderer_2d.h"

namespace SlimeBoom
{
void BushGenerator::UpdateBushPositions()
{
    // Num Bushesが実行中に変わったら、位置の数を合わせる(増えた分は次のループで配置し直される)
    if (m_bush_positions_.size() != m_num_bushes_)
    {
        m_bush_positions_.resize(m_num_bushes_, Vector3(FLT_MAX, 0.0f, FLT_MAX));
        m_is_dirty_ = true;
    }

    for (int i = 0; std::cmp_less(i, m_bush_positions_.size()); ++i)
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

    const auto material = m_bush_renderer_->shared_materials.empty()
                              ? nullptr
                              : m_bush_renderer_->shared_materials[0].CastedLock();
    if (material == nullptr || m_bush_positions_.empty())
        return;

    // シェーダーは送った位置の数だけインスタンスを描くので、実際に送る数に合わせる
    m_bush_renderer_->SetInstanceCount(static_cast<uint32_t>(m_bush_positions_.size()));
    material->SetBuffer("positions", m_bush_positions_.data(), sizeof(Vector3), m_bush_positions_.size());
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