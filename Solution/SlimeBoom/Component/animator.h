#pragma once
#include "Animation/animation_component.h"

class Animator : public engine::Component
{
    bool m_blending_ = false;
    engine::AssetPtr<engine::AnimationComponent> m_animation_component_;
    std::string m_before_anim_type_;
    std::string m_current_anim_type_;
    float m_blend_speed_ = 0;
    float m_blend_weight_ = 0.0f;
    std::multimap<std::string, std::shared_ptr<engine::AnimationState>> m_states_ = {};

public:
    void OnAwake() override;
    void OnUpdate() override;
    void OnInspectorGui() override;

    float BlendWeight() const;
    void SetBlendWeight(float weight);
    
    bool Blending() const;
    std::string GetCurrentAnimName();
    std::shared_ptr<engine::AnimationState> GetState(const std::string& anim_name);

    void StopAnim(const std::string& anim_name);
    void PlayAnim(const std::string& anim_name);
    void BlendAnim(const std::string& from_name, const std::string& to_name, float blend_speed = 0.0f);

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_states_),
            CEREAL_NVP(m_animation_component_)
        );
    }
};

CEREAL_CLASS_VERSION(Animator, 1)
