#include "pch.h"
#include "game_object.h"
#include "animator.h"

#include <algorithm>
#include <utility>

#include "engine_time.h"
#include "gui.h"

std::shared_ptr<engine::AnimationState> Animator::GetState(const std::string &anim_name)
{
    auto it = m_states_.find(anim_name);
    return it != m_states_.end() ? it->second : nullptr;
}

void Animator::StopAnim(const std::string &anim_name)
{
    auto state = GetState(anim_name);
    state->enabled = false;
    state->time = 0.0f;
    state->ReleaseCurvesCache();
}

void Animator::OnAwake()
{
    if (m_animation_component_.Lock() == nullptr)
    {
        m_animation_component_ = engine::AssetPtr<engine::AnimationComponent>::FromManaged(GameObject()->GetComponent<engine::AnimationComponent>());
    }

    auto animation_component = m_animation_component_.CastedLock();
    for (auto &[fst, snd] : m_states_)
    {
        if (snd->clip == nullptr)
            continue;
        animation_component->AddState(snd, fst);
        snd->length = snd->clip.CastedLock()->Length();
    }
}

void Animator::OnUpdate()
{
    if (m_current_anim_type_.empty() || GetState(m_current_anim_type_) == nullptr)
    {
        m_blending_ = false;
        return;
    }

    if (m_before_anim_type_ != m_current_anim_type_ && GetState(m_before_anim_type_) != nullptr)
    {
        m_blend_weight_ += m_blend_speed_ * engine::Time::GetDeltaTime();
        if (m_blend_weight_ >= 1.0f)
        {
            m_blending_ = false;
        }
    }
    else
    {
        m_blending_ = false;
    }

    ApplyWeights();
}

void Animator::StopOtherStates(const std::string &keep_a, const std::string &keep_b)
{
    for (auto &[name, state] : m_states_)
    {
        if (state == nullptr || name == keep_a || name == keep_b)
            continue;

        if (state->enabled)
        {
            state->enabled = false;
            state->time = 0.0f;
            state->ReleaseCurvesCache();
        }
        state->weight = 0.0f;
    }
}

void Animator::ApplyWeights()
{
    m_blend_weight_ = std::clamp(m_blend_weight_, 0.0f, 1.0f);

    const auto current_anim = GetState(m_current_anim_type_);
    if (current_anim == nullptr)
        return;

    const auto before_anim = m_before_anim_type_ == m_current_anim_type_
        ? nullptr
        : GetState(m_before_anim_type_);

    if (before_anim == nullptr || !before_anim->enabled)
    {
        current_anim->weight = 1.0f;
        return;
    }

    // ブレンド元とブレンド先の合計は常に1になる
    before_anim->weight = 1.0f - m_blend_weight_;
    current_anim->weight = m_blend_weight_;
}

void Animator::OnInspectorGui()
{
    auto animation = m_animation_component_.CastedLock();
    for (auto it = m_states_.begin(); it != m_states_.end();)
    {
        ImGui::PushID(it._Ptr);
        ImGui::Indent();
        engine::Gui::PropertyField("AnimationClip", it->second->clip);

        it->second->OnInspectorGui();

        char buf[256];
        strncpy_s(buf, sizeof(buf), it->first.c_str(), _TRUNCATE);
        buf[sizeof(buf) - 1] = '\0';

        if (ImGui::InputText("text", buf, sizeof(buf), 0))
        {
            // キーが変更された場合
            if (it->first != buf)
            {
                auto nh = m_states_.extract(it++);
                nh.key() = buf;
                m_states_.insert(std::move(nh));
            }
        }
        else
        {
            ++it;
        }
        ImGui::PopID();
        ImGui::Unindent();
    }

    if (ImGui::Button("Add"))
    {
        auto it = m_states_.emplace();
        it->second = std::make_shared<engine::AnimationState>();
    }
    if (ImGui::Button("Remove"))
        m_states_.erase(--m_states_.end());
}

float Animator::BlendWeight() const
{
    return m_blend_weight_;
}

void Animator::SetBlendWeight(const float weight)
{
    m_blend_weight_ = std::clamp(weight, 0.0f, 1.0f);
}

bool Animator::Blending() const
{
    return m_blending_;
}

std::string Animator::GetCurrentAnimName()
{
    return m_current_anim_type_;
}

void Animator::PlayAnim(const std::string &anim_name)
{
    const auto state = GetState(anim_name);
    if (state == nullptr)
        return;

    m_blending_ = false;
    StopOtherStates(anim_name, anim_name);

    m_before_anim_type_ = anim_name;
    m_current_anim_type_ = anim_name;
    m_blend_weight_ = 1.0f;
    state->enabled = true;

    ApplyWeights();
}

void Animator::BlendAnim(const std::string &from_name, const std::string &to_name, const float blend_speed)
{
    const auto from_state = GetState(from_name);
    const auto to_state = GetState(to_name);

    if (to_state == nullptr)
        return;
    if (from_state == nullptr || from_name == to_name)
    {
        PlayAnim(to_name);
        return;
    }

    if (from_name == m_current_anim_type_)
        m_blend_weight_ = 1.0f - m_blend_weight_;
    else if (from_name != m_before_anim_type_)
        m_blend_weight_ = 0.0f;
    m_blend_weight_ = std::clamp(m_blend_weight_, 0.0f, 1.0f);

    m_blend_speed_ = blend_speed;
    m_blending_ = true;

    StopOtherStates(from_name, to_name);

    m_before_anim_type_ = from_name;
    m_current_anim_type_ = to_name;

    if (to_state->wrap_mode != engine::kWrapMode::kLoop)
    {
        StopAnim(m_current_anim_type_);
    }

    from_state->enabled = true;
    to_state->enabled = true;

    ApplyWeights();
}

CEREAL_REGISTER_TYPE(Animator);