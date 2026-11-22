#include "pch.h"
#include "terrain_grid_component.h"

#include <numeric>
#include <random>

#include "game_object.h"
#include "gui.h"
#include "Components/mesh_renderer.h"
#include "Components/transform.h"
#include "Physics/mesh_collider.h"
#include "../Utils/gpu_buffer.h"

namespace SlimeBoom
{
namespace
{
//面の表裏をFBXから読み込んだMeshと同じ(法線側から見て反時計回り)にする。逆になっていたらtrueにする
constexpr bool kFlipWinding = false;

//シード値付きの2D Perlinノイズ
class PerlinNoise
{
    std::array<int, 512> m_perm_{};

    static float Fade(const float t)
    {
        return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
    }

    static float Grad(const int hash, const float x, const float y)
    {
        switch (hash & 7)
        {
        case 0: return x + y;
        case 1: return -x + y;
        case 2: return x - y;
        case 3: return -x - y;
        case 4: return x;
        case 5: return -x;
        case 6: return y;
        default: return -y;
        }
    }

public:
    explicit PerlinNoise(const uint32_t seed)
    {
        std::array<int, 256> p{};
        std::iota(p.begin(), p.end(), 0);
        std::mt19937 engine(seed);
        std::shuffle(p.begin(), p.end(), engine);
        for (int i = 0; i < 512; ++i)
            m_perm_[i] = p[i & 255];
    }

    //およそ-1..1の値を返す
    float Noise(const float x, const float y) const
    {
        const int xi = static_cast<int>(std::floor(x)) & 255;
        const int yi = static_cast<int>(std::floor(y)) & 255;
        const float xf = x - std::floor(x);
        const float yf = y - std::floor(y);
        const float u = Fade(xf);
        const float v = Fade(yf);

        const int aa = m_perm_[m_perm_[xi] + yi];
        const int ab = m_perm_[m_perm_[xi] + yi + 1];
        const int ba = m_perm_[m_perm_[xi + 1] + yi];
        const int bb = m_perm_[m_perm_[xi + 1] + yi + 1];

        const float x1 = std::lerp(Grad(aa, xf, yf), Grad(ba, xf - 1.0f, yf), u);
        const float x2 = std::lerp(Grad(ab, xf, yf - 1.0f), Grad(bb, xf - 1.0f, yf - 1.0f), u);
        return std::lerp(x1, x2, v);
    }
};

//Meshへ面を追加するヘルパー
struct MeshBuilder
{
    engine::Mesh& mesh;

    void AddVertex(const Vector3& position, const Vector3& normal, const Vector4& tangent, const Vector2& uv) const
    {
        mesh.vertices.emplace_back(position);
        mesh.normals.emplace_back(normal);
        mesh.tangents.emplace_back(tangent);
        mesh.uvs[0].emplace_back(uv);
    }

    void AddTriangle(uint32_t a, uint32_t b, uint32_t c, const Vector3& normal) const
    {
        const auto& v = mesh.vertices;
        const bool is_ccw = (v[b] - v[a]).Cross(v[c] - v[a]).Dot(normal) >= 0.0f;
        if (is_ccw == kFlipWinding)
            std::swap(b, c);
        mesh.indices.insert(mesh.indices.end(), {a, b, c});
    }

    //4頂点(外周順)のクワッドを追加する
    void AddQuad(const std::array<Vector3, 4>& positions, const std::array<Vector2, 4>& uvs,
                 const Vector3& normal, const Vector4& tangent) const
    {
        const auto base = static_cast<uint32_t>(mesh.vertices.size());
        for (int i = 0; i < 4; ++i)
            AddVertex(positions[i], normal, tangent, uvs[i]);
        AddTriangle(base, base + 1, base + 2, normal);
        AddTriangle(base, base + 2, base + 3, normal);
    }
};
}

std::array<float, 4> TerrainGridComponent::GetCornerLevels(const int x, const int z) const
{
    const int size = static_cast<int>(m_grid_size_);
    if (x < 0 || z < 0 || x >= size || z >= size)
        return {0.0f, 0.0f, 0.0f, 0.0f};

    const auto& cell = m_grid_[z][x];
    const float h = cell.height;
    // x0z0, x1z0, x0z1, x1z1
    std::array<float, 4> corners = {h, h, h, h};
    if (cell.type != kCellType::kSlope)
        return corners;

    switch (cell.direction)
    {
    case kSlopeDirection::kPositiveX:
        corners[1] = corners[3] = h + 1.0f;
        break;
    case kSlopeDirection::kNegativeX:
        corners[0] = corners[2] = h + 1.0f;
        break;
    case kSlopeDirection::kPositiveZ:
        corners[2] = corners[3] = h + 1.0f;
        break;
    case kSlopeDirection::kNegativeZ:
        corners[0] = corners[1] = h + 1.0f;
        break;
    default:
        break;
    }
    return corners;
}

void TerrainGridComponent::GenerateGrid()
{
    const int size = static_cast<int>(m_grid_size_);
    const PerlinNoise noise(m_seed_);

    m_grid_.assign(size, std::vector<CellData>(size));

    //ノイズから段数を決める
    for (int z = 0; z < size; ++z)
    {
        for (int x = 0; x < size; ++x)
        {
            const float n = noise.Noise(static_cast<float>(x) * m_noise_scale_,
                                        static_cast<float>(z) * m_noise_scale_);
            const float normalized = std::clamp(n * 0.5f + 0.5f, 0.0f, 1.0f);
            const float level = std::round(normalized * static_cast<float>(m_max_level_));
            m_grid_[z][x] = {kCellType::kFlat, level, kSlopeDirection::kNone};
        }
    }

    //上下左右に1段高いセルがあれば、そちらへ上る坂にする
    constexpr std::array<std::pair<int, int>, 4> kOffsets = {{{1, 0}, {-1, 0}, {0, 1}, {0, -1}}};
    constexpr std::array<kSlopeDirection, 4> kDirections = {
        kSlopeDirection::kPositiveX, kSlopeDirection::kNegativeX,
        kSlopeDirection::kPositiveZ, kSlopeDirection::kNegativeZ
    };
    for (int z = 0; z < size; ++z)
    {
        for (int x = 0; x < size; ++x)
        {
            auto& cell = m_grid_[z][x];
            for (int i = 0; i < 4; ++i)
            {
                const int nx = x + kOffsets[i].first;
                const int nz = z + kOffsets[i].second;
                if (nx < 0 || nz < 0 || nx >= size || nz >= size)
                    continue;
                if (m_grid_[nz][nx].height - cell.height == 1.0f)
                {
                    cell.type = kCellType::kSlope;
                    cell.direction = kDirections[i];
                    break;
                }
            }
        }
    }
}

void TerrainGridComponent::BuildMesh()
{
    m_mesh_ = engine::Object::Instantiate<engine::Mesh>("TerrainMesh");
    const MeshBuilder builder{*m_mesh_};

    const int size = static_cast<int>(m_grid_size_);
    const float cell = m_cell_size_;
    const float half = static_cast<float>(size) * cell * 0.5f;

    const auto corner_position = [&](const int x, const int z, const float level)
    {
        return Vector3(-half + static_cast<float>(x) * cell, level * m_level_height_, -half + static_cast<float>(z) * cell);
    };
    //壁のUVのV。上面(1セル=UV1)とテクスチャの密度を揃える
    const auto wall_v = [&](const float level)
    {
        return level * m_level_height_ / cell;
    };

    for (int z = 0; z < size; ++z)
    {
        for (int x = 0; x < size; ++x)
        {
            const auto c = GetCornerLevels(x, z);

            //上面(平地/坂)
            {
                const std::array<Vector3, 4> positions = {
                    corner_position(x, z, c[0]),
                    corner_position(x + 1, z, c[1]),
                    corner_position(x + 1, z + 1, c[3]),
                    corner_position(x, z + 1, c[2])
                };
                auto normal = (positions[2] - positions[0]).Cross(positions[3] - positions[1]);
                if (normal.y < 0.0f)
                    normal = -normal;
                normal.Normalize();
                auto tangent = positions[1] - positions[0];
                tangent.Normalize();

                const float u = static_cast<float>(x);
                const float v = static_cast<float>(z);
                builder.AddQuad(positions,
                                {Vector2(u, v), Vector2(u + 1, v), Vector2(u + 1, v + 1), Vector2(u, v + 1)},
                                normal, Vector4(tangent.x, tangent.y, tangent.z, 1.0f));
            }

            //側面(段差/崖/外周)。自分の辺が隣の辺より高い部分だけ壁を出す
            struct Edge
            {
                int dx, dz;
                //自分の辺の2隅、隣の対応する2隅(GetCornerLevelsの添字)
                int own0, own1, nb0, nb1;
                //辺の2隅のグリッド座標オフセット
                int ax, az, bx, bz;
            };
            constexpr std::array<Edge, 4> kEdges = {{
                {1, 0, 1, 3, 0, 2, 1, 0, 1, 1},
                {-1, 0, 0, 2, 1, 3, 0, 0, 0, 1},
                {0, 1, 2, 3, 0, 1, 0, 1, 1, 1},
                {0, -1, 0, 1, 2, 3, 0, 0, 1, 0},
            }};
            for (const auto& edge : kEdges)
            {
                const auto nb = GetCornerLevels(x + edge.dx, z + edge.dz);
                const float top0 = c[edge.own0];
                const float top1 = c[edge.own1];
                const float nb0 = nb[edge.nb0];
                const float nb1 = nb[edge.nb1];
                if (top0 <= nb0 && top1 <= nb1)
                    continue;

                const float bottom0 = std::min(top0, nb0);
                const float bottom1 = std::min(top1, nb1);
                const int ax = x + edge.ax, az = z + edge.az;
                const int bx = x + edge.bx, bz = z + edge.bz;

                const std::array<Vector3, 4> positions = {
                    corner_position(ax, az, bottom0),
                    corner_position(bx, bz, bottom1),
                    corner_position(bx, bz, top1),
                    corner_position(ax, az, top0)
                };
                const Vector3 normal(static_cast<float>(edge.dx), 0.0f, static_cast<float>(edge.dz));
                const Vector3 tangent(static_cast<float>(bx - ax), 0.0f, static_cast<float>(bz - az));
                const float u = static_cast<float>(edge.dx != 0 ? z : x);
                builder.AddQuad(positions,
                                {
                                    Vector2(u, wall_v(bottom0)), Vector2(u + 1, wall_v(bottom1)),
                                    Vector2(u + 1, wall_v(top1)), Vector2(u, wall_v(top0))
                                },
                                normal, Vector4(tangent.x, tangent.y, tangent.z, 1.0f));
            }
        }
    }

    m_mesh_->ReconstructMeshesBuffer();
}

void TerrainGridComponent::CreateTerrainObject() const
{
    const auto terrain = engine::Object::Instantiate<engine::GameObject>("TerrainMesh");
    terrain->Transform()->SetParent(GameObject()->Transform());
    terrain->Transform()->SetLocalPosition(Vector3::Zero);

    const auto mesh = engine::AssetPtr<engine::Mesh>::FromInstance(m_mesh_);

    const auto renderer = terrain->AddComponent<engine::MeshRenderer>();
    renderer->SetSharedMesh(mesh);
    if (m_material_.CastedLock() != nullptr)
        renderer->shared_materials.push_back(m_material_);

    const auto collider = terrain->AddComponent<engine::MeshCollider>();
    collider->SetMesh(mesh);
}

void TerrainGridComponent::UploadGpuData()
{
    const int size = static_cast<int>(m_grid_size_);
    std::vector<Vector4> heights;
    heights.reserve(static_cast<size_t>(size) * size);
    for (int z = 0; z < size; ++z)
    {
        for (int x = 0; x < size; ++x)
        {
            const auto& corners = GetCornerLevels(x, z);
            heights.emplace_back(corners[0], corners[1], corners[2], corners[3]);
        }
    }

    const float half = static_cast<float>(size) * m_cell_size_ * 0.5f;
    const Vector3 origin = GameObject()->Transform()->Position() + Vector3(-half, 0.0f, -half);

    m_heights_buffer_ = GpuBuffer::CreateUploadStructured(
        sizeof(Vector4), static_cast<uint32_t>(heights.size()), "terrain_heights");
    m_heights_buffer_.Update(heights.data(), sizeof(Vector4) * heights.size());
    engine::Material::SetGlobalBuffer("terrain_heights", m_heights_buffer_.GetHandle());
    engine::Material::SetGlobalInt("terrain_grid_size", size);
    engine::Material::SetGlobalFloat("terrain_cell_size", m_cell_size_);
    engine::Material::SetGlobalFloat("terrain_level_height", m_level_height_);
    engine::Material::SetGlobalVector3("terrain_origin", origin);
}

void TerrainGridComponent::OnInspectorGui()
{
    engine::Gui::PropertyField("Grid Size", m_grid_size_);
    engine::Gui::PropertyField("Cell Size", m_cell_size_);
    engine::Gui::PropertyField("Level Height", m_level_height_);
    engine::Gui::PropertyField("Seed", m_seed_);
    engine::Gui::PropertyField("Noise Scale", m_noise_scale_);
    engine::Gui::PropertyField("Max Level", m_max_level_);
    engine::Gui::PropertyField("Material", m_material_);
}

void TerrainGridComponent::OnStart()
{
    if (m_grid_size_ == 0)
        return;

    GenerateGrid();
    BuildMesh();
    CreateTerrainObject();
    UploadGpuData();
}

float TerrainGridComponent::GetHeight(const uint32_t x, const uint32_t y) const
{
    if (y >= m_grid_.size() || x >= m_grid_[y].size())
        return 0.0f;
    return m_grid_[y][x].height * m_level_height_;
}

uint32_t TerrainGridComponent::GetGridSize() const
{
    return m_grid_size_;
}

float TerrainGridComponent::GetCellSize() const
{
    return m_cell_size_;
}

float TerrainGridComponent::GetLevelHeight() const
{
    return m_level_height_;
}

const std::vector<std::vector<CellData>>& TerrainGridComponent::GetGrid() const
{
    return m_grid_;
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::TerrainGridComponent)
