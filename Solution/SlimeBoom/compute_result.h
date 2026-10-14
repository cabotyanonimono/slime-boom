#pragma once
#include "compute_result_types.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/compute_shader_component.h"
#include "Components/transform.h"
#include "Rendering/CabotEngine/Graphics/ByteAddressBuffer.h"

namespace SlimeBoom
{

// ComputeShaderが書き込んだ結果バッファをGPUからCPUへ読み戻し、項目名で値を取得できるようにするComponent。
class ComputeResult : public engine::Component
{
    bool m_is_first_frame_ = true;
    std::shared_ptr<engine::ByteAddressBuffer> m_compute_result_buffer_;
    size_t m_listener_token_ = -1;
    std::array<uint32_t, ComputeResultTypes::kOffset> m_result_ = {};
    
public:
    void OnStart() override;
    /// 読み戻しが完了していれば結果を取り込み、次の読み戻しをレンダリング時にリクエストする。
    void OnUpdate() override;
    void OnDestroy() override;

    /// 読み戻した結果から、項目の値へのポインタを返す。
    /// GPUからの読み戻しは非同期のため、最新フレームの値とは限らない。
    /// @param name ComputeResultTypes::kResultTypeOffsetsに定義された項目名
    /// @return 項目の先頭ポインタ。項目ごとの型(intやVector3など)にキャストして使う。
    void *GetValue(const std::string& name);
    
    template <class Archive>
    void serialize(Archive &ar, const uint32_t version)
    {
        ar(cereal::base_class<Component>(this));
    }
};
}

CEREAL_CLASS_VERSION(SlimeBoom::ComputeResult, 1)