#pragma once
#include "../UI/ui_button.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"

class SettingsView : public engine::Component
{
    engine::AssetPtr<ui::UIButton> m_window_mode_button_;
    engine::AssetPtr<ui::UIButton> m_fullscreen_button_;
    engine::AssetPtr<ui::UIButton> m_windowed_button_;
    engine::AssetPtr<ui::UIButton> m_fps_counter_button_;
    engine::AssetPtr<ui::UIButton> m_fps_thirty_;
    engine::AssetPtr<ui::UIButton> m_fps_sixty_;
    engine::AssetPtr<ui::UIButton> m_fps_one_hundred_and_forty_four_;
    engine::AssetPtr<ui::UIButton> m_fps_two_hundred_and_forty_;
    
public:
    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this)
        );
    }

    
};

CEREAL_CLASS_VERSION(SettingsView, 1)
