#pragma once
#include "Asset/asset_ptr.h"
#include "Components/mesh_renderer.h"

namespace SlimeBoom::ParticleQuadRenderer
{
/// GPUパーティクルを、四角形1枚 × パーティクル数のインスタンスとして描くようにrendererを設定する。
/// CaboRenderingSystemには頂点数だけで描く方法(ProceduralRenderer)が無いので、その代わりに使う。
/// シェーダー側は SV_InstanceID でパーティクルを引き、頂点の位置(xy: -1..1)を四角形の角として使う。
/// rendererはシーン側でカリングを切っておくこと（パーティクルは画面中どこにでも出るため）。
/// @param renderer 描画に使うMeshRenderer（マテリアルは設定済みのもの）
/// @param particle_count 描くパーティクルの数（インスタンス数）
/// @return 作った四角形のMesh。rendererが参照している間は持っておくこと
std::shared_ptr<engine::Mesh> Setup(const engine::AssetPtr<engine::MeshRenderer> &renderer, uint32_t particle_count);
}
