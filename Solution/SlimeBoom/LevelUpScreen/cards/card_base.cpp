#include "pch.h"
#include "card_base.h"

void SlimeBoom::CardBase::OnInspectorGui()
{
    m_card_data_.OnInspectorGui();
}

bool SlimeBoom::CardBase::CanSelect()
{
    return true;
}

std::string SlimeBoom::CardBase::GetEffectText()
{
    return {};
}

std::string SlimeBoom::CardBase::FormatNumber(const float value)
{
    // 小数第2位までで丸め、末尾の0と小数点を落とす（10.50 -> 10.5、3.00 -> 3）
    char buffer[32];
    std::snprintf(buffer, sizeof(buffer), "%.2f", value);
    std::string text = buffer;
    while (!text.empty() && text.back() == '0')
        text.pop_back();
    if (!text.empty() && text.back() == '.')
        text.pop_back();
    return text == "-0" ? "0" : text;
}

std::string SlimeBoom::CardBase::FormatChange(const float before, const float after, const std::string& prefix,
                                              const std::string& suffix)
{
    return prefix + FormatNumber(before) + suffix + "  ->  " + prefix + FormatNumber(after) + suffix;
}

SlimeBoom::CardData& SlimeBoom::CardBase::GetCardData()
{
    return m_card_data_;
}

CEREAL_REGISTER_TYPE(SlimeBoom::CardBase)
