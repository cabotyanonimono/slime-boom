#include "pch.h"
#include "scene_transition_executor.h"

#include "gui.h"

namespace SlimeBoom
{
void SceneTransitionExecutor::OnInspectorGui()
{
    engine::Gui::PropertyField("Scene Transition", m_scene_transition_controller_);
}

void SceneTransitionExecutor::OnAwake()
{
    if (const auto scene_transition = m_scene_transition_controller_.CastedLock())
    {
        scene_transition->Execute();
    }
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::SceneTransitionExecutor);