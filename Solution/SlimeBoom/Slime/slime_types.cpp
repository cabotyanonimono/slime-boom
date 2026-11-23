#include "pch.h"
#include "slime_types.h"

namespace SlimeBoom::SlimeTypes
{
std::vector<SlimeTypeDefinition> CreateDefaultDefinitions()
{
    SlimeTypeDefinition normal;
    normal.name = "Normal";
    normal.params.death_color = Color(0.0f, 0.353f, 1.0f, 1.0f);
    normal.params.texture_index = 0;

    SlimeTypeDefinition speed;
    speed.name = "Speed";
    speed.params.death_color = Color(1.0f, 1.0f, 1.0f, 1.0f);
    speed.params.speed_scale = 1.5f;
    speed.params.texture_index = 1;

    SlimeTypeDefinition tank;
    tank.name = "Tank";
    tank.params.death_color = Color(0.259f, 0.0f, 0.431f, 1.0f);
    tank.params.max_hp = 45;
    tank.params.size_scale = 1.5f;
    tank.params.texture_index = 2;

    return {normal, speed, tank};
}
}
