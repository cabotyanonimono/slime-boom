#ifndef COMPUTE_COMMON_HLSLI
#define COMPUTE_COMMON_HLSLI

// CaboRenderingSystemで動かすComputeShaderの共通定義
// - エントリーポイントは CSMain
// - 使えるレジスタは b0..b3 / t0..t7 / u0..u7 / s0..s3（すべてspace0）
// - 値は変数名で設定される（cbuffer名ではない）
//
// 定数レジスタの割り当て
//   b0 : SlimeGlobals（slime_globals.hlsli。ゲーム側がMaterial::SetGlobal〜で設定）
//   b1 : ComputeTime（ComputeMaterialが実行の直前に書き込む）
//   b2, b3 : シェーダーごとの値（MaterialBlockで設定）

cbuffer ComputeTime : register(b1)
{
    float time;      // 起動からの秒数
    float deltatime; // 前フレームからの秒数
}

// ComputeMaterialが既定のサンプラー（名前にClampを含むものはClamp、それ以外はWrap）を設定する
SamplerState SmpClamp : register(s0);

#endif
