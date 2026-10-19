#pragma once

namespace SlimeBoom
{
// 難易度(ステージ)。
enum class kStage
{
    kStage1 = 1,
    kStage2 = 2,
    kStage3 = 3
};

// タイトル画面で選ばれたステージを、シーンをまたいで保持する。
class StageSelection
{
    inline static kStage m_stage_ = kStage::kStage1;

public:
    /// 選択したステージを設定する。
    static void SetStage(kStage stage) { m_stage_ = stage; }
    /// 選択されているステージを返す。
    static kStage GetStage() { return m_stage_; }
};
}
