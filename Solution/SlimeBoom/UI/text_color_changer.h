#pragma once
#include "Components/component.h"
#include "Components/text_renderer.h"

namespace SlimeBoom
{
// 複数のTextRendererの色を、まとめて指定した色に変えて、あとで元の色に戻すComponent。
// ポーズ画面など、画面を暗くする演出のときに、暗転の影響を受けないテキストの明るさを合わせるために使う。
// 色の変更はRGBだけで、アルファ値は各テキストの元の値のまま残る。
class TextColorChanger final : public engine::Component
{
    std::vector<engine::AssetPtr<engine::TextRenderer>> m_texts_;
    Color m_changed_color_ = Color(0.5f, 0.5f, 0.5f, 1.0f);
    // Apply時点の各テキストの色。m_texts_と同じ順番で保存する。
    std::vector<Color> m_original_colors_;
    bool m_is_changed_ = false;

public:
    void OnInspectorGui() override;

    /// 対象のテキストの元の色を保存してから、RGBを変更後の色に変える。すでに変更中のときは何もしない。
    void Apply();
    /// Applyで保存した元の色にテキストを戻す。変更中でないときは何もしない。
    void Restore();

    /// Applyで色を変更している最中かを返す。
    [[nodiscard]] bool IsChanged() const;
    /// 変更後の色を設定する。変更中の場合は、次のApplyから反映される。
    void SetChangedColor(const Color& color);

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_texts_),
            CEREAL_NVP(m_changed_color_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::TextColorChanger, 1)
