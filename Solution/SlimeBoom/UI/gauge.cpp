#include "pch.h"
#include "gauge.h"

#include "engine_time.h"
#include "gui.h"
#include "Components/rect_transform.h"

namespace SlimeBoom
{
void Gauge::ApplyImageSize() const
{
    const auto bar_rect = m_bar_image_.CastedLock()->GameObject()->GetComponent<engine::RectTransform>();

    bar_rect->anchored_position = m_box_.pos;
    bar_rect->size_delta = m_box_.extents;

    if (m_bar_frame_image_ != nullptr)
    {
        const auto frame_rect = m_bar_frame_image_.CastedLock()->GameObject()->GetComponent<engine::RectTransform>();

        frame_rect->anchored_position = m_box_.pos;
        frame_rect->size_delta = m_box_.extents - Vector2::One;
    }
}

void Gauge::OnInspectorGui()
{
    engine::Gui::PropertyField("Bar Image", m_bar_image_);
    engine::Gui::PropertyField("Bar Frame Image", m_bar_frame_image_);
    engine::Gui::PropertyField("Ratio", m_ratio_);

    bool is_box_resized = false;
    if (engine::Gui::PropertyField("BarPos", m_box_.pos))
    {
        is_box_resized = true;
    }

    if (engine::Gui::PropertyField("BarExtents", m_box_.extents))
    {
        is_box_resized = true;
    }

    if (is_box_resized)
    {
        ApplyImageSize();
    }
}

void Gauge::OnUpdate()
{
    auto constant_buffer = m_bar_image_.CastedLock()->shared_material.CastedLock()->shared_material_block->
                                        GetConstantBufferData("Ratio");
    constant_buffer->SetFloatData("ratio", m_ratio_);
}

float Gauge::Ratio() const
{
    return m_ratio_;
}

void Gauge::SetRatio(const float ratio)
{
    m_ratio_ = ratio;
}

Gauge::Gauge() : m_box_(Vector2(100, 100), Vector2(100, 100))
{
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::Gauge)
