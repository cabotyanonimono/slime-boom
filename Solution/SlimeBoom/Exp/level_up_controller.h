#pragma once
#include "event.h"
#include "../compute_result.h"
#include "Asset/asset_ptr.h"

namespace SlimeBoom
{
class LevelUpController : public engine::Component
{
    engine::AssetPtr<ComputeResult> m_compute_result_;
    int m_current_exp_ = 0;
    int m_current_level_exp_ = 0;
    int m_current_level_ = 0;
    int m_required_exp_ = 0;
    int m_base_required_exp_ = 50;
    float m_required_exp_up_rate_ = 1.1f;
    
    engine::Event<int> m_on_level_up_;

    int CalcRequiredExp();

public:
    void OnInspectorGui() override;
    void OnStart() override;
    void OnUpdate() override;

    size_t AddOnLevelUpListener(const std::function<void(int)>& callback);
    void RemoveOnLevelUpListener(size_t token);

    /// @return 現在の総獲得EXP
    int GetCurrentExp() const;
    /// @return 現在のレベルで獲得したEXP。
    int GetCurrentLevelExp() const;
    int GetCurrentLevel() const;
    int GetRequiredExp() const;

    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this)
        );

        if (version >= 2)
        {
            ar(
                CEREAL_NVP(m_compute_result_)
            );
        }
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::LevelUpController, 2)
