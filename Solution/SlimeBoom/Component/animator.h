#pragma once
#include "Animation/animation_component.h"

// AnimationComponentのAnimationStateを名前で管理し、再生・停止・ブレンドを行うComponent。
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
    /// AnimationComponentが未設定なら同じGameObjectから取得し、登録済みのStateをAnimationComponentへ追加する。
    void OnAwake() override;
    void OnUpdate() override;
    void OnInspectorGui() override;

    /// 現在のブレンドの重み(0~1)を返す。1に近いほどブレンド先のアニメーションが強く反映される。
    float BlendWeight() const;
    /// ブレンドの重みを外部から直接設定する。
    void SetBlendWeight(float weight);
    
    /// ブレンド中かどうかを返す。
    bool Blending() const;
    /// 再生中(ブレンド先)のアニメーション名を返す。未再生の場合は空文字。
    std::string GetCurrentAnimName();
    /// @param anim_name アニメーション名
    /// @return 該当するAnimationState。存在しない場合はnullptr。
    std::shared_ptr<engine::AnimationState> GetState(const std::string& anim_name);

    /// 指定したアニメーションを無効化し、再生時間を0に戻す。
    void StopAnim(const std::string& anim_name);
    /// ブレンドせずに指定したアニメーションを即座に再生する。直前のアニメーションは停止する。
    void PlayAnim(const std::string& anim_name);
    /// from_nameからto_nameへブレンドしながら切り替える。
    /// @param from_name ブレンド元のアニメーション名
    /// @param to_name ブレンド先のアニメーション名
    /// @param blend_speed 1秒あたりに増えるブレンドの重み。大きいほど素早く切り替わる。
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
