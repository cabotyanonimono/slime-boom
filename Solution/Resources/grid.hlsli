#ifndef TERRAIN_HLSLI
#define TERRAIN_HLSLI

// TerrainGridComponent::UploadGpuData が Material::SetGlobal〜 で変数名ごとに設定する。
// CRS の space0 は b0..b3 までしか無いので、値は1つの cbuffer にまとめて b3 に置く。
// t0 は player_attacks が使っているので t1 に置く。

// terrain_heights[z * terrain_grid_size + x] がセル(x, z)の4隅の段数(x0z0, x1z0, x0z1, x1z1)
StructuredBuffer<float4> terrain_heights : register(t1);

cbuffer TerrainGrid : register(b3)
{
    float3 terrain_origin;
    int terrain_grid_size;
    float terrain_cell_size;
    float terrain_level_height;
}

//ワールド座標の地面の高さを返す。グリッドの外の場合OriginのY座標を返す。
float GetTerrainHeight(float x, float z)
{
    if (terrain_grid_size <= 0 || terrain_cell_size <= 0.0)
        return terrain_origin.y;

    float2 grid = (float2(x, z) - terrain_origin.xz) / terrain_cell_size;

    // グリッド外ならベースの高さ
    if (any(grid < 0.0) || any(grid >= (float)terrain_grid_size))
        return terrain_origin.y;

    // どのセルか(整数部)と、セル内のどこか(小数部 0..1)
    int2   cell = (int2)floor(grid);
    float2 f    = grid - cell;

    // 4隅を取り出して bilinear 補間
    float4 c = terrain_heights[cell.y * terrain_grid_size + cell.x];
    float z0 = lerp(c.x, c.y, f.x); // z0 の辺
    float z1 = lerp(c.z, c.w, f.x); // z1 の辺
    float level = lerp(z0, z1, f.y);

    // 段数 → ワールド高さ
    return terrain_origin.y + level * terrain_level_height;
}

#endif
