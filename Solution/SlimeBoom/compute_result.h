#pragma once
#include "compute_result_types.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/compute_shader_component.h"
#include "Components/transform.h"
#include "Rendering/CabotEngine/Graphics/ByteAddressBuffer.h"

namespace SlimeBoom
{

class ComputeResult : public engine::Component
{
    bool m_is_first_frame_ = true;
    std::shared_ptr<engine::ByteAddressBuffer> m_compute_result_buffer_;
    size_t m_listener_token_ = -1;
    std::array<uint32_t, ComputeResultTypes::kOffset> m_result_ = {};
    
public:
    void OnStart() override;
    void OnUpdate() override;
    void OnDestroy() override;

    void *GetValue(const std::string& name);
    
    template <class Archive>
    void serialize(Archive &ar, const uint32_t version)
    {
        ar(cereal::base_class<Component>(this));
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::ComputeResult, 1)