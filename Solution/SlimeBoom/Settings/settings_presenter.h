#pragma once
#include "settings_model.h"
#include "Components/component.h"

// 設定データ(SettingsModel)を保持するComponent。
class SettingsPresenter : public engine::Component
{
    SettingsModel m_model_;
    
public:
    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this)
        );
    }
};

CEREAL_CLASS_VERSION(SettingsPresenter, 1)
