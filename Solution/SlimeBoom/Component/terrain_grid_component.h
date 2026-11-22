#pragma once
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Rendering/material.h"
#include "Rendering/mesh.h"
#include <CaboRenderingSystem/RHI/gpu_resources.h>

namespace SlimeBoom
{
enum class kCellType
{
    kFlat,
    kSlope
};

//坂の上り方向。平地はkNone
enum class kSlopeDirection
{
    kNone = -1,
    kPositiveX,
    kNegativeX,
    kPositiveZ,
    kNegativeZ
};

struct CellData
{
    kCellType type = kCellType::kFlat;
    float height = 0.0f;
    kSlopeDirection direction = kSlopeDirection::kNone;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(CEREAL_NVP(type), CEREAL_NVP(height), CEREAL_NVP(direction));
    }
};

//Terrainの生成、管理を行うComponent。
//グリッドの中心はこのGameObjectのTransform位置になる
class TerrainGridComponent : public engine::Component
{
    std::vector<std::vector<CellData>> m_grid_;

    uint32_t m_grid_size_ = 100;
    float m_cell_size_ = 50.0f;
    float m_level_height_ = 1.0f;
    uint32_t m_seed_ = 0;
    float m_noise_scale_ = 0.05f;
    int m_max_level_ = 4;
    engine::AssetPtr<engine::Material> m_material_;

    std::shared_ptr<engine::Mesh> m_mesh_;
    CRS::Buffer m_heights_buffer_;

    //Gridの生成を行う
    //上下左右の高さの差が1マスだった場合坂になる
    void GenerateGrid();
    //Gridから全セルを結合した1枚のMeshを作る
    void BuildMesh();
    //子GameObjectを作りMeshRendererとMeshColliderを付ける
    void CreateTerrainObject() const;
    //シェーダーから参照する高さマップバッファを登録する
    void UploadGpuData();

    //セルの4隅の段数(x0z0, x1z0, x0z1, x1z1)を取得する。範囲外は0
    std::array<float, 4> GetCornerLevels(int x, int z) const;

public:
    void OnInspectorGui() override;
    //Gridの生成を行い、それに対応したCell(Mesh,Collider)の生成、GPUDataの生成を行う
    void OnStart() override;

    //セル(x, y)の基準面のワールド高さ(Transformからの相対)。範囲外は0
    float GetHeight(uint32_t x, uint32_t y) const;
    uint32_t GetGridSize() const;
    float GetCellSize() const;
    float GetLevelHeight() const;
    const std::vector<std::vector<CellData>>& GetGrid() const;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_grid_size_),
            CEREAL_NVP(m_cell_size_),
            CEREAL_NVP(m_seed_),
            CEREAL_NVP(m_noise_scale_),
            CEREAL_NVP(m_max_level_),
            CEREAL_NVP(m_material_)
        );

        //version 1は高さもcell_sizeで決めていたので、同じ見た目になるように合わせる
        if (version >= 2)
            ar(CEREAL_NVP(m_level_height_));
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::TerrainGridComponent, 2)
