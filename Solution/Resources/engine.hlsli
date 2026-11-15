#ifndef __ENGINE_HLSLI__
#define __ENGINE_HLSLI__

// 旧エンジン向けに書いたShaderを、CaboRenderingSystem(CRS)で動かすための互換レイヤー。
// 実体は Shaders/crs_common.hlsli（エンジンの値は space1、マテリアルの値は space0）。
// 新しく書くShaderは crs_common.hlsli を直接includeすること。
//
// - エントリーポイントは VSMain / PSMain（影を描くなら VSShadow）
// - マテリアルの cbuffer / Texture は register を省略してよい（space0 の b0..b3 / t0..t7 / s0..s3 に自動で割り当てられる）

#include "Shaders/crs_common.hlsli"

// エンジンのVertexと同じ並びの頂点入力（旧エンジンの名前のまま）
struct VSInput
{
    float3 pos : POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float4 tangent : TANGENT;
    float2 uv : TEXCOORD0;
    float2 uv2 : TEXCOORD1;
    float2 uv3 : TEXCOORD2;
    float2 uv4 : TEXCOORD3;
    float2 uv5 : TEXCOORD4;
    float2 uv6 : TEXCOORD5;
    float2 uv7 : TEXCOORD6;
    float2 uv8 : TEXCOORD7;
    uint bones_per_vertex : BONESPERVERTEX;
    uint4 bone_id : BONEINDEX;
    float4 bone_weight : BONEWEIGHT;
};

// ---- 旧エンジンの名前 → CRSの名前 ----
#define World world
#define View view
#define Proj projection
#define BoneMatrices bone_matrices
#define LightViewProj light_view_projections
#define ShadowMaps shadow_maps
#define shadowSampler shadow_sampler
#define SmpClamp linear_clamp_sampler

// 時間は View の time（x: 起動からの時間, y: 前フレームからの時間）
static float deltatime = time.y;

// カメラの位置と向き（viewはC++の行列のままなので、3行目がカメラの後ろ向き）
static float3 camera_pos = camera_position.xyz;
static float3 camera_dir = normalize(-view[2].xyz);

// マテリアルのサンプラー（既定は線形補間・Wrap。MaterialのInspectorで変えられる）
SamplerState smp : register(s3);

// 環境光 (RenderingSettings で設定)
float3 AmbientLight()
{
    return ambient_color.rgb;
}

// PBRシェーダー用の環境光 (拡散 + 鏡面)
// Basic系シェーダー(albedo × ambient)と同じ絶対量になるよう、マテリアルの intensity は掛けない
// 金属は kD が小さいので、鏡面成分(F)で環境光を受ける
float3 PbrAmbientLight(float3 albedo, float3 f, float metallic)
{
    float3 kd = (1.0f - f) * (1.0f - metallic);
    return (kd * albedo + f) * AmbientLight();
}

#endif
