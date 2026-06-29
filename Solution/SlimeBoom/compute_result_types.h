#pragma once

struct NameSizePair
{
    std::string_view name;
    size_t size;
};

struct ComputeResultTypes
{
    static constexpr auto kSize = 4;
    static constexpr auto kOffset = 5;
    static constexpr std::array<NameSizePair, kSize> kResultTypeOffsets =
    {
        {
            {"closest_slime_pos", 3},
            {"taken_damage", 1},
            {"exp", 1}
        }
    };

    static size_t GetComputeResultBufferSize();
    static size_t GetComputeResultOffset(const std::string& name);
};
