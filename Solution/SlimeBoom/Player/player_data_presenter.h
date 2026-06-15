#pragma once
#include "player_attack_data.h"
#include "Components/component.h"
#include "Rendering/structured_buffer_data.h"
#include "Rendering/CabotEngine/Graphics/ConstantBuffer.h"

namespace SlimeBoom
{
class PlayerDataPresenter : public engine::Component
{
    engine::AssetPtr<engine::Transform> m_player_transform_;
    
    int m_max_attack_data_count_ = 1;
    size_t m_lisner_token_ = -1;
    std::vector<PlayerAttackData> m_attack_data_;
    std::shared_ptr<engine::StructuredBufferData> m_attack_data_buffer_;
    std::shared_ptr<engine::ConstantBuffer> m_attack_data_count_buffer_;

public:
    void OnInspectorGui() override;
    void OnStart() override;
    void OnUpdate() override;

    void SetAttackData(const PlayerAttackData &player_attack_data);

    template <class Archive>
    void serialize(Archive &ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_max_attack_data_count_),
            CEREAL_NVP(m_player_transform_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::PlayerDataPresenter, 2)