#pragma once
#include "condition_base.h"

// 状態の遷移条件の集合。すべての条件を満たしたとき(AND)に遷移できる。
class Transition
{
    std::unordered_set<std::shared_ptr<ConditionBase>> m_conditions_;

public:
    /// すべての条件に、参照するステートマシンを設定する。
    void SetStateMachine(const std::shared_ptr<StateMachine>& state_machine) const;
    /// 条件を追加する。
    void AddCondition(std::shared_ptr<ConditionBase> condition);
    /// 条件を削除する。
    void RemoveCondition(const std::shared_ptr<ConditionBase>& condition);
    /// すべての条件を満たしているかを返す。
    bool Evaluate() const;
};
