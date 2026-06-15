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
    
    [[nodiscard]] int Int() const
    {
        return std::any_cast<int>(value);
    }
    [[nodiscard]] float Float() const
    {
        return std::any_cast<float>(value);
    }
    [[nodiscard]] bool Bool() const
    {
        return std::any_cast<bool>(value);
    }
};
