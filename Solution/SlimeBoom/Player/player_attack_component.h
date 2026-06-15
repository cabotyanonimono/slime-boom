#pragma once
#include "player_attack_data.h"
#include "player_data_presenter.h"
#include "player_sword_controller.h"
#include "slash_effect_component.h"
#include "Components/component.h"
#include "Components/mesh_renderer.h"
#include "Rendering/CabotEngine/Graphics/StructuredBuffer.h"

namespace SlimeBoom
{
class PlayerAttackComponent : public engine::Component
{
    engine::AssetPtr<engine::Transform> m_player_transform_;
    engine::AssetPtr<PlayerDataPresenter> m_attack_data_presenter_;
    engine::AssetPtr<SlashEffectComponent> m_slash_effect_component_;
    engine::AssetPtr<PlayerSwordController> m_sword_controller_;
    PlayerAttackData m_player_attack_data_ = {};

    float m_cooldown_ = 0.0f;
    float m_cooldown_timer_ = 0.0f;

    bool m_is_first_frame_ = true;
    std::shared_ptr<engine::StructuredBuffer> m_closest_slime_buffer_;
    size_t m_lisner_token_ = -1;
    Vector3 m_closest_slime_pos_;

    void FetchClosestSlimePos();
    
public:
    void OnInspectorGui() override;
    void OnStart() override;
    void OnUpdate() override;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_player_attack_data_)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_player_transform_),
                CEREAL_NVP(m_attack_data_presenter_)
            );
        }

        if (version >= 3)
        {
            ar(
                CEREAL_NVP(m_slash_effect_component_)
            );
        }

        if (version >= 4)
        {
            ar(
                CEREAL_NVP(m_sword_controller_)
            );
        }
        
        if (version >= 5)
        {
            ar(
                CEREAL_NVP(m_cooldown_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::PlayerAttackComponent, 5)