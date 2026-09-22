#pragma once
#include "Asset/asset_ptr.h"
#include "Components/camera_component.h"
#include "Components/component.h"

class SetNullRenderTextureComponent : public engine::Component
{
    bool m_is_enable_ = true;
    bool m_is_first_frame_ = true;
    engine::AssetPtr<engine::CameraComponent> m_camera_;
    
public:
    void OnInspectorGui() override;
    void OnUpdate() override;
    
    template <class Archive>
    void serialize(Archive& ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this),
            CEREAL_NVP(m_camera_)
        );
    }
};

CEREAL_CLASS_VERSION(SetNullRenderTextureComponent, 1)
