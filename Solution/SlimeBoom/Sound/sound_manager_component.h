#pragma once
#include "sound_types.h"
#include "Asset/asset_ptr.h"
#include "Audio/audio_source_component.h"
#include "Components/component.h"

namespace SlimeBoom
{
// 効果音を種類(kSoundTypes)ごとに保持し、どこからでも再生・停止できるようにするComponent。
class SoundManagerComponent : public engine::Component
{
    inline static std::array<engine::AssetPtr<engine::AudioSourceComponent>, static_cast<size_t>(kSoundTypes::kSoundTypesCount)> m_sounds_;
    
public:
    void OnInspectorGui() override;
    /// ゲームが停止した際にサウンドの再生を停止する
    void OnUpdate() override;
    /// 指定した種類の効果音を再生する。
    /// @return 効果音が登録されていて再生できたらtrue
    static bool Play(const kSoundTypes sound_type);
    /// 指定した種類の効果音が再生中かを返す。
    /// @return 再生中ならtrue。効果音が未登録の場合もfalse。
    static bool IsPlaying(const kSoundTypes sound_type);
    /// 指定した種類の効果音を停止する。
    /// @return 効果音が登録されていて停止できたらtrue
    static bool Stop(const kSoundTypes sound_type);
    
    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
                cereal::base_class<Component>(this)
            );
        if (version >= 4)
        {
            ar(
                CEREAL_NVP(m_sounds_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::SoundManagerComponent, 4)