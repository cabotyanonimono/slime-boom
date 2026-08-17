#pragma once

struct NameSizePair
{
    std::string_view name;
    size_t size;
};

struct ComputeResultTypes
{
    static constexpr auto kSize = 5;
    static constexpr auto kOffset = 7;
    static constexpr std::array<NameSizePair, kSize> kResultTypeOffsets =
    {
        {
            {"closest_slime_pos", 3},
            {"taken_damage", 1},
            {"exp", 1},
            {"eliminate_slime_count", 1},
            {"deal_damage_count", 1}
        }
    };

    static size_t GetComputeResultBufferSize();
    static size_t GetComputeResultOffset(const std::string& name);
};
