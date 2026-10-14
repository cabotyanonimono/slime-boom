#pragma once
#include <random>

/// minからmaxまでの一様乱数を返す。Tが浮動小数点型なら実数、整数型なら整数の乱数になる。
template <typename T>
static T Random(T min, T max)
{
    static std::mt19937 s_engine{std::random_device{}()};

    if constexpr (std::is_floating_point_v<T>)
    {
        std::uniform_real_distribution<T> dist(min, max);
        return dist(s_engine);
    }
    else
    {
        std::uniform_int_distribution<T> dist(min, max);
        return dist(s_engine);
    }
}
