#include "pch.h"
#include "sound_manager_component.h"

#include "gui.h"
#include "../game_time.h"

namespace SlimeBoom
{
void SoundManagerComponent::OnInspectorGui()
{
    for (int i = 0; i < m_sounds_.size(); ++i)
    {
        ImGui::PushID(i);
        engine::Gui::PropertyField(ToString(static_cast<kSoundTypes>(i)), m_sounds_[i]);
        ImGui::PopID();
    }
}

void SoundManagerComponent::OnUpdate()
{
    if (GameTime::IsGameStopped())
    {
        for (const auto& sound : m_sounds_)
        {
            if (sound == nullptr)
                continue;
            
            sound->Stop();
        }
    }
}

bool SoundManagerComponent::Play(const kSoundTypes sound_type)
{
    const auto sound = m_sounds_[static_cast<int>(sound_type)];
    if (sound == nullptr)
        return false;
    sound->Play();
    return true;
}

bool SoundManagerComponent::IsPlaying(const kSoundTypes sound_type)
{
    const auto sound = m_sounds_[static_cast<int>(sound_type)];
    if (sound == nullptr)
        return false;

    return sound->IsPlaying();
}

bool SoundManagerComponent::Stop(const kSoundTypes sound_type)
{
    const auto sound = m_sounds_[static_cast<int>(sound_type)];
    if (sound == nullptr)
        return false;

    sound->Stop();
    return true;
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::SoundManagerComponent)
