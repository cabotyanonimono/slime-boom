#pragma once
#include "ui_data_provider.h"
#include "../LevelUpScreen/card_type.h"
#include "Components/renderer.h"
#include "Rendering/font_data.h"
#include "Rendering/material.h"

namespace SlimeBoom
{
// レベルアップ画面で、カードの左に持っているスキルのアイコンを、右にプレイヤーのステータス(アイコンと数値)を縦に並べて描くRenderer。
// シーンには置かず、LevelUpCardControllerがOnStartで自分のGameObjectに追加する。
// アイコンはカードのアイコン画像(カード全体の大きさで、上の方にアイコンがある透明な画像)をそのまま使う。
class LevelUpStatusView final : public engine::Renderer
{
    struct Row
    {
        kCardType card_type;
        std::string value;
    };

    std::weak_ptr<UiDataProvider> m_ui_data_provider_;
    std::shared_ptr<engine::FontData> m_font_;
    // アイコンごとのMaterial。_MainTexはMaterialごとに1枚なので、カードの種類ごとに作る
    std::unordered_map<kCardType, std::shared_ptr<engine::Material>> m_icon_materials_;

    /// カードの種類のアイコンを描くMaterialを返す。無ければカードのアイコン画像から作る。
    std::shared_ptr<engine::Material> GetIconMaterial(kCardType card_type);
    /// 持っているスキルの行を返す。
    std::vector<Row> CollectSkillRows() const;
    /// プレイヤーのステータスの行を返す。
    std::vector<Row> CollectStatusRows() const;

    /// Canvasの基準解像度を返す(Canvasが無い場合は2304x1296)。
    Vector2 CanvasSize() const;
    /// アイコンの見えている部分が、Canvas上のcenter(中央が原点、上が+y)にsizeの正方形で出るように描く。
    void DrawIcon(kCardType card_type, Vector2 center, float size);
    /// Canvas上のposition(中央が原点、上が+y)を基準に文字を描く。pivot_xは0で左揃え、0.5で中央揃え。
    void DrawText(const std::string& text, Vector2 position, float height, float pivot_x, const Color& color) const;

public:
    /// 表示に使うデータの提供元を設定する。
    void Setup(const std::shared_ptr<UiDataProvider>& ui_data_provider);

    /// スキルとステータスの一覧を描く。
    void Render() override;
    /// 作ったアイコンのMaterialを破棄する(Objectの一覧に残り続けるので、明示的に破棄する)。
    void OnDestroy() override;
    /// 画面に出すUIなので、boundsの基準はMain CameraのWorldMatrixにする(TextRendererと同じ)。
    Matrix BoundsOrigin() override;
};
}
