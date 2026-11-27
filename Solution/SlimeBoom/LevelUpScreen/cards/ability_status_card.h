#pragma once
#include "card_base.h"
#include "../../Player/player_data_component.h"
#include "../../Player/ability/ability_processer.h"
#include "../../Player/ability/ability_data/ability_types.h"

namespace SlimeBoom
{
// 特定のアビリティのステータス倍率(攻撃力・攻撃範囲・攻撃速度)を強化するカードの基底クラス。
// 対象のアビリティを習得しているときだけ抽選の候補になる。
// シリアライズは継承先で行う(このクラスのメンバーもそこで書き出す)。
class AbilityStatusCard : public CardBase
{
protected:
    float m_value_ = 0.1f;
    engine::AssetPtr<AbilityProcesser> m_ability_processer_;
    engine::AssetPtr<PlayerDataComponent> m_player_data_;

    /// 強化対象のアビリティの種類。
    [[nodiscard]] virtual kAbilityTypes GetAbilityType() const = 0;
    /// 強化するステータスの種類。
    [[nodiscard]] virtual kAbilityStatus GetStatusType() const = 0;
    /// 強化対象のアビリティを習得しているかを返す。
    virtual bool HasTargetAbility() = 0;

public:
    void OnInspectorGui() override;
    /// 対象アビリティのステータス倍率にm_value_を加える。
    void OnSelect() override;
    /// 選んだときの変化を「x現在の倍率  ->  x選択後の倍率」で返す。
    std::string GetEffectText() override;
    /// 対象のアビリティを習得済みの場合のみ選択可能。
    bool CanSelect() override;
};
}
