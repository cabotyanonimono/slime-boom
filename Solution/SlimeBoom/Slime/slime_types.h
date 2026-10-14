#pragma once

// スライムの種類の定義。
namespace SlimeTypes
{
inline static constexpr int Normal = 0;
inline static constexpr int Speed = 1;
inline static constexpr int Tank = 2;
inline static constexpr int Count = 3;

/// スライムの種類を文字列に変換する。
inline std::string ToString(const int type)
{
    switch (type)
    {
        case Normal:
        return "Normal";
        case Speed:
        return "Speed";
        case Tank:
        return "Tank";
    }
    return "";
}
}