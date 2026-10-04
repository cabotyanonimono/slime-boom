#include "pch.h"
#include "slime_eliminate_se.h"

#include "gui.h"
#include "../Sound/sound_manager_component.h"

namespace SlimeBoom
{
void SlimeEliminateSe::OnInspectorGui()
{
    engine::Gui::PropertyField("Slime Eliminate Checker", m_slime_eliminate_checker_);
}

void SlimeEliminateSe::OnStart()
{
    m_slime_eliminate_checker_->AddOnEliminateListener(
        [this](int eliminate_count)
        {
            for (int i = 0; i < eliminate_count; ++i)
                SoundManagerComponent::Play(kSoundTypes::kSlimeEliminate);
        });
}
}

CEREAL_REGISTER_TYPE(SlimeBoom::SlimeEliminateSe)
