#pragma once

struct NameSizePair
{
    std::string_view name;
    size_t size;
};

struct ComputeResultTypes
{
    static constexpr auto kSize = 3;
    static constexpr std::array<NameSizePair, kSize> kResultTypeOffsets =
    {
        {
            {"closest_slime_pos", 3},
            {"damage", 1},
            {"exp", 1},
        }
    };

    static size_t GetComputeResultBufferSize();
    static size_t GetComputeResultOffset(const std::string& name);
};
