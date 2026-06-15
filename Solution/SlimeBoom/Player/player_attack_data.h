#pragma once

namespace SlimeBoom
{
struct PlayerAttackData
{
    float arc;
    float size;
    Vector3 angle;
    float damage;
    float power;
    
    template <class Archive>
    void serialize(Archive &ar, const uint32_t version)
    {
        ar(
            CEREAL_NVP(arc),
            CEREAL_NVP(size),
            CEREAL_NVP(angle),
            CEREAL_NVP(damage),
            CEREAL_NVP(power)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::PlayerAttackData, 1)