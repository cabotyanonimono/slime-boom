#pragma once
#include "Components/component.h"

// インスペクターのボタンから、現在のアクティブシーンを破棄するデバッグ用のComponent。
class SceneChanger : public engine::Component
{
public:
    void OnInspectorGui() override;

    template <class Archive>
    void serialize(Archive &ar, const uint32_t version)
    {
        ar(
            cereal::base_class<Component>(this)
        );
    }
};

CEREAL_CLASS_VERSION(SceneChanger, 1)