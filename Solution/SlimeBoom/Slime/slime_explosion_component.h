#pragma once
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/compute_shader_component.h"
#include "Rendering/CabotEngine/Graphics/ConstantBuffer.h"
#include "Rendering/CabotEngine/Graphics/StructuredBuffer.h"

namespace SlimeBoom
{
// 連鎖爆発1回分のGPU用データ。slime_explosion.hlsliのSlimeExplosionと合わせること。
struct SlimeExplosionData
{
    float position[3];
    uint32_t generation;
    uint32_t frame;
    float padding[3];
};

// 連鎖爆発のパラメーター。シェーダーのcbuffer ExplosionParamsと合わせること。
struct SlimeExplosionParams
{
    float chance;
    float radius;
    float damage;
    float knockback;
    float chain_decay;
    uint32_t max_generation;
    uint32_t frame;
    uint32_t enabled;
    float damage_color_time;
};

// スライム撃破時の連鎖爆発を、GPU(ComputeShader)で処理するComponent。
// 倒れたスライムが確率で爆発を登録し、次のフレームにその範囲内のスライムへダメージを与える。
class SlimeExplosionComponent : public engine::Component
{
    static constexpr uint32_t kMaxExplosionCount = 128;
    static constexpr uint32_t kMaxSlimesCount = 8096;
    static constexpr uint32_t kThreadGroupSize = 64;

    std::shared_ptr<engine::StructuredBuffer> m_explosion_buffer_;
    std::shared_ptr<engine::ConstantBuffer> m_params_buffer_;
    engine::AssetPtr<engine::ComputeShaderComponent> m_explosion_compute_;

    SlimeExplosionParams m_params_ = {};
    float m_chance_ = 0.3f;
    float m_chain_decay_ = 0.7f;
    float m_damage_color_time_ = 0.2f;

    // デバッグ表示用。GPUから読み戻した爆発バッファ
    std::array<SlimeExplosionData, kMaxExplosionCount> m_debug_explosions_ = {};
    size_t m_listener_token_ = -1;
    bool m_is_first_frame_ = true;

public:
    void OnStart() override;
    void OnDestroy() override;
    void OnInspectorGui() override;
    /// パラメーターをGPUに転送し、爆発のダメージ判定用ComputeShaderを実行する。
    void OnUpdate() override;

    /// 連鎖爆発の有効・無効を切り替える。
    void SetEnabled(bool enabled);
    /// 爆発の性能を設定する。
    /// @param radius 爆発の半径
    /// @param damage 爆発1回のダメージ
    /// @param knockback ノックバックの強さ
    /// @param max_generation 連鎖する最大段数
    /// @param chance_multiplier 基本の発動確率に掛ける倍率
    void SetExplosion(float radius, float damage, float knockback, uint32_t max_generation, float chance_multiplier);

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_explosion_compute_),
            CEREAL_NVP(m_chance_),
            CEREAL_NVP(m_chain_decay_),
            CEREAL_NVP(m_damage_color_time_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::SlimeExplosionComponent, 1)
