#pragma once
#include "player_attack_data.h"
#include "Components/component.h"
#include "Components/compute_shader_component.h"
#include "Rendering/structured_buffer_data.h"
#include "Rendering/CabotEngine/Graphics/ConstantBuffer.h"

namespace SlimeBoom
{
class PlayerDataPresenter : public engine::Component
{
    engine::AssetPtr<engine::Transform> m_player_transform_;
    
    int m_max_attack_data_count_ = 1;
    float m_position_y_offset_ = 0.0f;
    size_t m_lisner_token_ = -1;
    std::vector<PlayerAttackData> m_attack_data_;
    std::shared_ptr<engine::StructuredBuffer> m_attack_data_buffer_;
    std::shared_ptr<engine::ConstantBuffer> m_attack_data_count_buffer_;
    engine::AssetPtr<engine::ComputeShaderComponent> m_player_attack_shader_;
    std::unordered_set<size_t> m_attack_frames_;

    void ExecuteAttack();
    
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

        if (version >= 3)
        {
            ar(CEREAL_NVP(m_player_attack_shader_));
        }

        if (version >= 4)
        {
            ar(CEREAL_NVP(m_position_y_offset_));
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::PlayerDataPresenter, 4)