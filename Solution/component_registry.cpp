#include "pch.h"
#include "component_registry.h"

#include "component_factory.h"
#include "SlimeBoom/Component/camera_controller.h"
#include "SlimeBoom/Component/delay_follower.h"
#include "SlimeBoom/Component/mesh_renderer_generator_test.h"
#include "SlimeBoom/Component/root_motion_controller.h"
#include "SlimeBoom/Player/player_position_presenter.h"
#include "SlimeBoom/Player/player_component.h"
#include "SlimeBoom/compute_result.h"
#include "SlimeBoom/Component/game_manager.h"
#include "SlimeBoom/Exp/exp_simulation_component.h"
#include "SlimeBoom/Exp/level_up_controller.h"
#include "SlimeBoom/Slime/slime_simulation_component.h"
#include "SlimeBoom/Player/player_data_presenter.h"
#include "SlimeBoom/Player/player_sword_controller.h"
#include "SlimeBoom/Player/slash_effect_component.h"
#include "SlimeBoom/Player/ability/ability_data/ability_data.h"
#include "SlimeBoom/UI/exp_gauge.h"
#include "SlimeBoom/UI/gauge.h"
#include "SlimeBoom/UI/player_hp_gauge.h"
#include "SlimeBoom/UI/survival_timer.h"
#include "SlimeBoom/UI/ui_button.h"
#include "SlimeBoom/UI/ui_data_provider.h"
#include "SlimeBoom/UI/ui_navigator.h"
#include "SlimeBoom/Wave/wave_controller.h"

namespace SlimeBoom
{
void ComponentRegistry::RegisterComponents()
{
#define ADD_COMPONENT(type, category) engine::IComponentFactory::Register(std::make_shared<engine::ComponentFactory<type>>(category))

    ADD_COMPONENT(DelayFollower, "Camera");
    ADD_COMPONENT(CameraController, "Camera");
    ADD_COMPONENT(SlimeSimulationComponent, "Slime");
    ADD_COMPONENT(PlayerComponent, "Player");
    ADD_COMPONENT(PlayerPositionPresenter, "Player");
    ADD_COMPONENT(RootMotionController, "Animation");
    ADD_COMPONENT(Animator, "Animation");
    ADD_COMPONENT(MeshRendererGenerator, "Renderer");
    ADD_COMPONENT(ComputeResult, "Debug");
    ADD_COMPONENT(PlayerDataPresenter, "Player");
    ADD_COMPONENT(SlashEffectComponent, "Player");
    ADD_COMPONENT(PlayerSwordController, "Player");
    ADD_COMPONENT(AbilityData, "Player");
    ADD_COMPONENT(ComputeResult, "ComputeShader");
    ADD_COMPONENT(ExpSimulationComponent, "Exp");
    ADD_COMPONENT(WaveController, "Slime");
    ADD_COMPONENT(ui::UIButton, "UI");
    ADD_COMPONENT(ui::UINavigator, "UI");
    ADD_COMPONENT(SlimeBoom::GameManager, "SlimeBoom");
    ADD_COMPONENT(SurvivalTimer, "UI");
    ADD_COMPONENT(LevelUpController, "Exp");
    ADD_COMPONENT(Gauge, "UI");
    ADD_COMPONENT(ExpGauge, "UI");
    ADD_COMPONENT(PlayerDamageDispatcher, "Player");
    ADD_COMPONENT(UiDataProvider, "UI");
    ADD_COMPONENT(PlayerHpGauge, "UI");
}
}
