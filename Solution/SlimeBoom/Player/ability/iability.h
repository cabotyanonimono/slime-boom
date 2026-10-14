#pragma once

// プレイヤーのアビリティのインターフェース。AbilityProcesserによって管理される。
class IAbility
{
public:
    virtual ~IAbility() = default;
    /// AbilityProcesserに追加されたときに呼ばれる。
    virtual void OnAttach();
    /// AbilityProcesserから削除されるときに呼ばれる。
    virtual void OnDetach();
    virtual void Update(float delta_time);
    virtual void FixedUpdate();
};
