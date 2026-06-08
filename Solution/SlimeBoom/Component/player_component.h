#pragma once
#include "Components/component.h"

namespace SlimeBoom
{
class PlayerComponent final : public engine::Component
{
public:
    float speed;
    float health;
    float attack_speed;
    float attack_damage;

    void OnInspectorGui() override;
    
    template <class Archive>
    void serialize(Archive &ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(speed),
            CEREAL_NVP(health),
            CEREAL_NVP(attack_speed),
            CEREAL_NVP(attack_damage)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::PlayerComponent, 1)