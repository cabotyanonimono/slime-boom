#pragma once
#include "sound_types.h"
#include "Asset/asset_ptr.h"
#include "Audio/audio_source_component.h"
#include "Components/component.h"

namespace SlimeBoom
{
class SoundManagerComponent : public engine::Component
{
    inline static std::array<engine::AssetPtr<engine::AudioSourceComponent>, static_cast<size_t>(kSoundTypes::kSoundTypesCount)> m_sounds_;
    
public:
    void OnInspectorGui() override;
    static bool Play(const kSoundTypes sound_type);
    static bool IsPlaying(const kSoundTypes sound_type);
    static bool Stop(const kSoundTypes sound_type);
    
    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
                cereal::base_class<Component>(this)
            );
        if (version >= 2)
        {
            ar(
                cereal::base_class<Component>(this),
                CEREAL_NVP(m_sounds_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::SoundManagerComponent, 2)