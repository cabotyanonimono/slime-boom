#pragma once
#include "../Player/player_component.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"

namespace SlimeBoom
{
class GameManager : public engine::Component
{
    float m_time_since_game_start_ = 0.0f;
    
public:

    void OnUpdate() override;
    
    float TimeSinceGameStart() const;
    
    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(cereal::base_class<Component>(this));
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::GameManager, 1)
