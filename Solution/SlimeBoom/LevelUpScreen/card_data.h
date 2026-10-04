#pragma once
#include "Asset/asset_ptr.h"
#include "Rendering/CabotEngine/Graphics/Texture2D.h"

namespace SlimeBoom
{
struct CardData : engine::Inspectable
{
    engine::AssetPtr<engine::Texture2D> icon;
    engine::AssetPtr<engine::Texture2D> name;
    engine::AssetPtr<engine::Texture2D> description;
    engine::AssetPtr<engine::Texture2D> background;

    void OnInspectorGui() override;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            CEREAL_NVP(icon)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(name),
                CEREAL_NVP(description)
            );
        }

        if (version >= 3)
        {
            ar(
                CEREAL_NVP(background)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::CardData, 3)