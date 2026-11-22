#include "pch.h"
#include "root_motion_controller.h"

#include "gui.h"
#include "Physics/rigidbody_component.h"

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
    if (!m_is_motion_enabled_)
        return;

    const auto rigidbody = GameObject()->GetComponent<engine::RigidbodyComponent>();
    if (rigidbody != nullptr && rigidbody->IsDynamic())
    {
        const auto parent = transform->Parent();
        m_pending_delta_ += parent != nullptr ? Vector3::TransformNormal(delta_pos, parent->LocalToWorld()) : delta_pos;
        return;
    }

    transform->SetLocalPosition(transform->LocalPosition() + delta_pos);
}

Vector3 RootMotionController::ConsumePendingDelta()
{
    const auto delta = m_pending_delta_;
    m_pending_delta_ = Vector3::Zero;
    return delta;
}

void RootMotionController::SetEnabledMotion(const bool enabled)
{
    m_is_motion_enabled_ = enabled;
    if (!enabled)
        m_pending_delta_ = Vector3::Zero;
}

CEREAL_REGISTER_TYPE(RootMotionController);
