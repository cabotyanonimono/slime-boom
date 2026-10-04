#pragma once
#include <any>

struct Parameter
{
    std::any value;
    void SetBool(bool value)
    {
        this->value = value;
    }
    void SetInt(int value)
    {
        this->value = value;
    }
    void SetFloat(float value)
    {
        this->value = value;
    }
    void SetVector3(Vector3 value)
    {
        this->value = value;
    }
    
    [[nodiscard]] int GetInt() const
    {
        return std::any_cast<int>(value);
    }
    [[nodiscard]] float GetFloat() const
    {
        return std::any_cast<float>(value);
    }
    [[nodiscard]] bool GetBool() const
    {
        return std::any_cast<bool>(value);
    }
    [[nodiscard]] Vector3 GetVector3() const
    {
        return std::any_cast<Vector3>(value);
    }
};
