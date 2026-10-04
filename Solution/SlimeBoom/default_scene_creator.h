#pragma once
#include "Coroutine/task.h"

namespace SlimeBoom
{
class DefaultSceneCreator
{
    static engine::Task DelayLoadSceneTask();
    
public:
    static void CreateDefaultScene();
};
}
