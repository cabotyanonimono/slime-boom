#pragma once

struct Box
{
    Vector2 pos;
    Vector2 extents;

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