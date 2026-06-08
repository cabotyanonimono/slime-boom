#include "pch.h"
#include "player_position_presenter.h"

#include "Rendering/gpu_resource_manager.h"

void SlimeBoom::PlayerPositionPresenter::OnInspectorGui()
{
    engine::Gui::PropertyField("Player Transform", m_player_transform_);
}

void SlimeBoom::PlayerPositionPresenter::OnUpdate()
{
    engine::GpuResourceManager::SetGlobalVector("PlayerPos", m_player_transform_->Position() + Vector3(0.0f, 0.5f, 0.0f));
}

CEREAL_REGISTER_TYPE(SlimeBoom::PlayerPositionPresenter)