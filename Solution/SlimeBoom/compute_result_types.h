#pragma once

// ComputeShaderの結果バッファ内の項目名と、その要素数の組。
struct NameSizePair
{
    std::string_view name;
    size_t size;
};

// ComputeShaderが書き込む結果バッファのレイアウト定義。
// kResultTypeOffsetsの並び順がそのままバッファ上の並び順になる。
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

    /// 結果バッファの合計サイズ(全項目のsizeの合計)を返す。
    static size_t GetComputeResultBufferSize();
    /// 指定した項目の、バッファ先頭からのオフセットを返す。
    /// @param name kResultTypeOffsetsに定義された項目名
    static size_t GetComputeResultOffset(const std::string& name);
};
