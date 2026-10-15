#include "pch.h"
#include "result_rank.h"

#include "engine_time.h"
#include "gui.h"

namespace SlimeBoom
{
void ResultRank::OnInspectorGui()
{
    engine::Gui::PropertyField("Rank Text", m_rank_text_);
    engine::Gui::PropertyField("S Threshold", m_s_threshold_);
    engine::Gui::PropertyField("A Threshold", m_a_threshold_);
    engine::Gui::PropertyField("B Threshold", m_b_threshold_);
    engine::Gui::PropertyField("C Threshold", m_c_threshold_);
    engine::Gui::PropertyField("Pop Scale", m_pop_scale_);
    engine::Gui::PropertyField("Pop Duration Sec", m_pop_duration_sec_);
}

void ResultRank::Hide()
{
    const auto rank_text = m_rank_text_.CastedLock();
    if (!rank_text)
        return;

    // 演出の途中で呼ばれても基準がずれないよう、最初の1回だけ記録する
    if (!m_is_base_scale_recorded_)
    {
        m_base_scale_ = rank_text->scale;
        m_is_base_scale_recorded_ = true;
    }
    rank_text->GameObject()->SetActive(false);
}

const char* ResultRank::CalcRank(const int eliminate_count) const
{
    if (eliminate_count >= m_s_threshold_)
        return "S";
    if (eliminate_count >= m_a_threshold_)
        return "A";
    if (eliminate_count >= m_b_threshold_)
        return "B";
    if (eliminate_count >= m_c_threshold_)
        return "C";
    return "D";
}

engine::Task ResultRank::Show(const int eliminate_count)
{
    const auto rank_text = m_rank_text_.CastedLock();
    if (!rank_text)
        co_return;

    if (!m_is_base_scale_recorded_)
    {
        m_base_scale_ = rank_text->scale;
        m_is_base_scale_recorded_ = true;
    }

    rank_text->string = CalcRank(eliminate_count);
    rank_text->scale = m_base_scale_ * m_pop_scale_;
    rank_text->GameObject()->SetActive(true);

    const float duration = (std::max)(m_pop_duration_sec_, 0.01f);
    float elapsed = 0.0f;

    while (elapsed < duration)
    {
        elapsed += engine::Time::Get()->UnscaledDeltaTime();
        const float t = (std::min)(elapsed / duration, 1.0f);
        const float eased = 1.0f - (1.0f - t) * (1.0f - t);
        rank_text->scale = m_base_scale_ * (m_pop_scale_ + (1.0f - m_pop_scale_) * eased);
        co_await engine::WaitForNextFrame();
    }

    rank_text->scale = m_base_scale_;

    co_return;
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::ResultRank)
