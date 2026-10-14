#pragma once

// 2Dの矩形。位置と大きさ(半径ではなく全体サイズ)を持つ。UIゲージの描画領域に使う。
struct Box
{
    Vector2 pos;
    Vector2 extents;

    /// @param pos 矩形の位置
    /// @param extents 矩形の大きさ
    Box(Vector2 pos, Vector2 extents);

    template <class Archive>
    void serialize(Archive &ar, const uint32_t version)
    {
        ar(
            CEREAL_NVP(pos),
            CEREAL_NVP(extents)
        );
    }
};

CEREAL_CLASS_VERSION(Box, 1)