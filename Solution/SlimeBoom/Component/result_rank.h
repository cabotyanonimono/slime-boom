#pragma once
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/text_renderer.h"
#include "Coroutine/task.h"

namespace SlimeBoom
{
// リザルト画面の評価ランク(S/A/B/C/D)を判定して表示するComponent。
// 撃破数を閾値と比べてランクを決め、大きく出してから基準の大きさに縮める演出で表示する。
// 表示の開始はClearSceneControllerから、Hide→Showの順で呼ばれる。
class ResultRank final : public engine::Component
{
    engine::AssetPtr<engine::TextRenderer> m_rank_text_;
    int m_s_threshold_ = 300;
    int m_a_threshold_ = 200;
    int m_b_threshold_ = 100;
    int m_c_threshold_ = 50;
    float m_pop_scale_ = 3.0f;
    float m_pop_duration_sec_ = 0.3f;

    float m_base_scale_ = 1.0f;
    bool m_is_base_scale_recorded_ = false;

public:
    void OnInspectorGui() override;

    /// ランクテキストを隠す。演出の前に呼ぶ。
    void Hide();
    /// 撃破数から評価ランクの文字列を返す。
    [[nodiscard]] const char* CalcRank(int eliminate_count) const;
    /// ランクを判定し、大きく出してから基準の大きさに縮めて表示するコルーチン。
    /// TimeScaleが0の間も動くよう、UnscaledDeltaTimeで進める。
    engine::Task Show(int eliminate_count);

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_rank_text_),
            CEREAL_NVP(m_s_threshold_),
            CEREAL_NVP(m_a_threshold_),
            CEREAL_NVP(m_b_threshold_),
            CEREAL_NVP(m_c_threshold_),
            CEREAL_NVP(m_pop_scale_),
            CEREAL_NVP(m_pop_duration_sec_)
        );
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::ResultRank, 1)