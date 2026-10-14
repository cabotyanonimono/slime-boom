#pragma once
#include "../box.h"
#include "Asset/asset_ptr.h"
#include "Components/image.h"

namespace SlimeBoom
{
// 画像でバーを描画する汎用のゲージ。割合と不透明度を設定できる。
class Gauge : public engine::Component
{
    float m_ratio_;
    float m_alpha_ = 1.0f;
    
    Box m_box_;
    engine::AssetPtr<engine::Image> m_bar_image_;
    engine::AssetPtr<engine::Image> m_bar_frame_image_;

    /// Boxの位置と大きさを、バーとフレームのRectTransformに反映する。
    void ApplyImageSize() const;

public:
    /// Boxをデフォルト値で初期化する。
    Gauge();
    
    void OnInspectorGui() override;
    void OnUpdate() override;


    /// ゲージの割合(0~1)を返す。
    float Ratio() const;
    /// ゲージの割合(0~1)を設定する。
    void SetRatio(float ratio);
    /// バーとフレームの不透明度を設定する。
    void SetAlpha(float alpha);
    
    template <class Archive>
    void serialize(Archive &ar, const uint32_t version)
    {
        ar(cereal::base_class<Component>(this),
           CEREAL_NVP(m_box_),
           CEREAL_NVP(m_bar_image_),
           CEREAL_NVP(m_bar_frame_image_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::Gauge, 1)