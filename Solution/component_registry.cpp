#include "pch.h"
#include "component_registry.h"

#include "component_factory.h"
#include "SlimeBoom/Component/camera_controller.h"
#include "SlimeBoom/Component/delay_follower.h"
#include "SlimeBoom/Component/mesh_renderer_generator_test.h"
#include "SlimeBoom/Component/root_motion_controller.h"
#include "SlimeBoom/Player/player_attack_component.h"
#include "SlimeBoom/Player/player_controller.h"
#include "SlimeBoom/Player/player_position_presenter.h"
#include "SlimeBoom/Slime/slime_damage_counter.h"
#include "SlimeBoom/Slime/slime_simulation_component.h"
#include "SlimeBoom/Player/player_data_presenter.h"
#include "SlimeBoom/Player/player_sword_controller.h"
#include "SlimeBoom/Player/slash_effect_component.h"

namespace SlimeBoom
{
void ComponentRegistry::RegisterComponents()
{
#define ADD_COMPONENT(type, category) engine::IComponentFactory::Register(std::make_shared<engine::ComponentFactory<type>>(category))

    ADD_COMPONENT(DelayFollower, "Camera");
    ADD_COMPONENT(CameraController, "Camera");
    ADD_COMPONENT(SlimeSimulationComponent, "Slime");
    ADD_COMPONENT(PlayerComponent, "Player");
    ADD_COMPONENT(PlayerController, "Player");
    ADD_COMPONENT(PlayerPositionPresenter, "Player");
    ADD_COMPONENT(RootMotionController, "Animation");
    ADD_COMPONENT(Animator, "Animation");
    ADD_COMPONENT(MeshRendererGenerator, "Renderer");
    ADD_COMPONENT(SlimeDamageCounter, "Debug");
    ADD_COMPONENT(PlayerAttackComponent, "Player");
    ADD_COMPONENT(PlayerDataPresenter, "Player");
    ADD_COMPONENT(SlashEffectComponent, "Player");
    ADD_COMPONENT(PlayerSwordController, "Player");
}
}
