#pragma once
#include "Asset/asset_ptr.h"
#include "Audio/audio_source_component.h"
#include "Components/component.h"

namespace ui
{
// UIボタンが選択された・押し込まれたときに効果音を鳴らすComponent。
// ボタンを使うシーンに1つ置き、選択音・押下音のAudioSourceを設定して使う。
class UIButtonSound : public engine::Component
{
    engine::AssetPtr<engine::AudioSourceComponent> m_select_sound_;
    engine::AssetPtr<engine::AudioSourceComponent> m_press_sound_;

    std::optional<size_t> m_selected_token_;
    std::optional<size_t> m_pressed_token_;

    /// 指定したAudioSourceが設定されていれば再生する。
    static void PlaySound(const engine::AssetPtr<engine::AudioSourceComponent>& sound);

public:
    void OnInspectorGui() override;
    /// ボタンの選択・押下イベントに効果音の再生を登録する。
    void OnEnabled() override;
    /// OnEnabledで登録したイベントを解除する。
    void OnDisabled() override;

    template <class Archive>
    void serialize(Archive& archive, const uint32_t version)
    {
        archive(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_select_sound_),
            CEREAL_NVP(m_press_sound_)
        );
    }
};
}

CEREAL_CLASS_VERSION(ui::UIButtonSound, 1);
