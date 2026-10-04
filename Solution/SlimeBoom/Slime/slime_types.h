#pragma once

namespace SlimeTypes
{
inline static constexpr int Normal = 0;
inline static constexpr int Speed = 1;
inline static constexpr int Tank = 2;
inline static constexpr int Count = 3;

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