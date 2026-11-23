#pragma once

namespace SlimeBoom
{
struct SlimeTypeParams
{
    Color death_color = Color(1.0f, 1.0f, 1.0f, 1.0f); // 撃破パーティクルの色
    Color albedo_tint = Color(1.0f, 1.0f, 1.0f, 1.0f);
    uint32_t max_hp = 15;
    float speed_scale = 1.0f;
    float size_scale = 1.0f;
    uint32_t texture_index = 0;

    template <class Archive>
    void serialize(Archive &ar, const uint32_t version)
    {
        ar(
            CEREAL_NVP(death_color),
            CEREAL_NVP(albedo_tint),
            CEREAL_NVP(max_hp),
            CEREAL_NVP(speed_scale),
            CEREAL_NVP(size_scale),
            CEREAL_NVP(texture_index)
        );
    }
};
static_assert(sizeof(SlimeTypeParams) == 48, "SlimeTypeParams must match SlimeTypeParams in slime.hlsli");

// スライム1種類分の定義(インスペクターで編集する名前とパラメータ)。
// 種類の番号(SlimeData::slime_type)は、SlimeSimulationComponentの種類の表の並び順になる。
struct SlimeTypeDefinition
{
    std::string name;
    SlimeTypeParams params;

    template <class Archive>
    void serialize(Archive &ar, const uint32_t version)
    {
        ar(
            CEREAL_NVP(name),
            CEREAL_NVP(params)
        );
    }
};

namespace SlimeTypes
{
inline constexpr int kMaxCount = 32;

std::vector<SlimeTypeDefinition> CreateDefaultDefinitions();
}
}

CEREAL_CLASS_VERSION(SlimeBoom::SlimeTypeParams, 0)
CEREAL_CLASS_VERSION(SlimeBoom::SlimeTypeDefinition, 0)
