#include "pch.h"
#include "gui.h"
#include "text_color_changer.h"


namespace SlimeBoom
{
void TextColorChanger::OnInspectorGui()
{
    engine::Gui::PropertyField("Texts", m_texts_);
    engine::Gui::PropertyField("Changed Color", m_changed_color_);
}

void TextColorChanger::Apply()
{
    if (m_is_changed_)
        return;

    m_original_colors_.clear();
    m_original_colors_.reserve(m_texts_.size());

    for (auto& text_ptr : m_texts_)
    {
        const auto text = text_ptr.CastedLock();
        // 参照が切れていても、m_original_colors_の並びがm_texts_とずれないよう、色だけ詰めておく。
        m_original_colors_.emplace_back(text ? text->color : Color());

        if (text == nullptr)
            continue;

        text->color = Color(m_changed_color_.x, m_changed_color_.y, m_changed_color_.z, text->color.w);
    }

    m_is_changed_ = true;
}

void TextColorChanger::Restore()
{
    if (!m_is_changed_)
        return;

    for (size_t i = 0; i < m_texts_.size() && i < m_original_colors_.size(); ++i)
    {
        if (const auto text = m_texts_[i].CastedLock())
            text->color = m_original_colors_[i];
    }

    m_original_colors_.clear();
    m_is_changed_ = false;
}

bool TextColorChanger::IsChanged() const
{
    return m_is_changed_;
}

void TextColorChanger::SetChangedColor(const Color& color)
{
    m_changed_color_ = color;
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::TextColorChanger)
