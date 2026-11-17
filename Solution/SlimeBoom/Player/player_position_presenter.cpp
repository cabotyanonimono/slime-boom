#include "pch.h"
#include "player_position_presenter.h"

#include "gui.h"
#include "Rendering/material.h"

void SlimeBoom::PlayerPositionPresenter::OnInspectorGui()
{
    engine::Gui::PropertyField("Player Transform", m_player_transform_);
}

void SlimeBoom::PlayerPositionPresenter::OnUpdate()
{
    engine::Material::SetGlobalVector3("player_pos", m_player_transform_->Position() + Vector3(0.0f, 0.5f, 0.0f));
}

CEREAL_REGISTER_TYPE(SlimeBoom::PlayerPositionPresenter)