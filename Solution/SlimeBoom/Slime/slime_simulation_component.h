#pragma once
#include "slime_take_damage_event_component.h"
#include "slime_types.h"
#include "Components/component.h"
#include "Components/compute_shader_component.h"
#include "Components/mesh_renderer.h"
#include "Rendering/CabotEngine/Graphics/ByteAddressBuffer.h"
#include "Rendering/CabotEngine/Graphics/StructuredBuffer.h"

namespace SlimeBoom
{
// スライム1体分のGPU用データ(位置・回転・スケール・速度・角速度・HP・被ダメージ色のタイマー)。
struct SlimeData
{
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    Vector3 velocity;
    Vector3 angular_velocity;
    uint32_t hp;
    float damage_color_timer;
};

// 全スライムのシミュレーションを、GPU(ComputeShader)で行うComponent。
// スライムの生成・物理・最寄りのスライムの検索・プレイヤーの攻撃判定を担当する。
class SlimeSimulationComponent : public engine::Component
{
    static constexpr int kMaxSlimesCount = 8096;
    int m_current_slime_count_ = 512;
    std::shared_ptr<engine::StructuredBuffer> m_slime_buffer_;
    std::shared_ptr<engine::ConstantBuffer> m_slime_count_buffer_;
    engine::AssetPtr<engine::ComputeShaderComponent> m_slime_generator_compute_;
    engine::AssetPtr<engine::ComputeShaderComponent> m_slime_physics_compute_;
    engine::AssetPtr<engine::ComputeShaderComponent> m_closest_slime_compute_;
    engine::AssetPtr<engine::ComputeShaderComponent> m_player_attack_compute_;
    engine::AssetPtr<engine::MeshRenderer> m_slime_renderer_;
    engine::AssetPtr<SlimeTakeDamageEventComponent> m_slime_take_damage_event_component_;

public:
    void OnStart() override;
    void OnInspectorGui() override;
    void OnUpdate() override;

    /// 現在のスライムの数を返す。
    int GetCurrentSlimesCount() const;
    /// スライムの数を設定する。最大数を超える場合は最大数に制限され、ComputeShaderのスレッドグループ数と描画のインスタンス数にも反映される。
    void SetCurrentSlimesCount(int count);
    /// スライムの種類ごとの出現率を設定する。
    /// @param rates SlimeTypesの種類ごとの出現率
    void SetSpawnRate(std::array<float, SlimeTypes::Count> rates) const;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this)
        );

        if (version >= 3)
        {
            ar(
                CEREAL_NVP(m_slime_generator_compute_),
                CEREAL_NVP(m_slime_physics_compute_),
                CEREAL_NVP(m_closest_slime_compute_),
                CEREAL_NVP(m_player_attack_compute_)
            );
        }

        if (version >= 4)
        {
            ar(
                CEREAL_NVP(m_slime_renderer_)
            );
        }

        if (version >= 5)
        {
            ar(
                CEREAL_NVP(m_slime_take_damage_event_component_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::SlimeSimulationComponent, 5)