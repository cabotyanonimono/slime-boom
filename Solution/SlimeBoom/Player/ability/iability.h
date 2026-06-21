#pragma once

class IAbility
{
public:
    virtual ~IAbility() = default;
    virtual void OnAttach();
    virtual void OnDetach();
    virtual void Update(float delta_time);
    virtual void FixedUpdate();
};
