#include "pch.h"
#include "root_motion_controller.h"

#include "gui.h"

void RootMotionController::OnInspectorGui()
{
    engine::Gui::PropertyField("Animation Component", m_animation_);
}

void RootMotionController::OnUpdate()
{
    const auto transform = GameObject()->Transform();
    const auto animation_component = m_animation_.CastedLock();
    const auto model_transform = animation_component->GameObject()->Transform();

    auto delta_pos = animation_component->GetDeltaPosition() * model_transform->Scale();

    transform->SetLocalPosition(transform->LocalPosition() + delta_pos);
}

CEREAL_REGISTER_TYPE(RootMotionController);