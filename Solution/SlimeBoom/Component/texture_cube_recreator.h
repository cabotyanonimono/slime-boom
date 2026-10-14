#pragma once
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Rendering/material.h"

namespace SlimeBoom
{
// マテリアルのキューブマップテクスチャを再生成させるComponent。
// Awake時と、最初のフレームが過ぎたあとの2回実行する。
class TextureCubeRecreator : public engine::Component
{
    std::vector<engine::AssetPtr<engine::Material>> m_materials_;
    bool m_is_first_frame_ = true;
    bool m_is_executed_ = false;

    /// 対象マテリアルのキューブマップ(env_textureまたはtexture_cube)を再生成の対象(dirty)にする。
    void ReCreateTextureCube();
public:
    void OnInspectorGui() override;
    void OnAwake() override;
    /// 最初のフレームが過ぎたあと、一度だけ再生成を行う。
    void OnUpdate() override;
    
    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_materials_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::TextureCubeRecreator, 2)
