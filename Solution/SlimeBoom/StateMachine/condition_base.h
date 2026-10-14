#pragma once

class StateMachine;

// 遷移条件の基底クラス。
class ConditionBase
{
protected:
    std::weak_ptr<StateMachine> m_state_machine_;

public:
    virtual ~ConditionBase() = default;
    /// 条件を満たしているかを返す。
    virtual bool Evaluate() const = 0;

    /// 条件がパラメーターを参照するステートマシンを設定する。
    void SetStateMachine(const std::shared_ptr<StateMachine>& state_machine);
};
