#pragma once
#include "compute_result_types.h"
#include "Asset/asset_ptr.h"
#include "Components/component.h"
#include "Components/compute_shader_component.h"
#include "Components/transform.h"
#include "event_receivers.h"
#include <CaboRenderingSystem/RHI/gpu_resources.h>

namespace SlimeBoom
{

// ComputeShaderが書き込んだ結果バッファをGPUからCPUへ読み戻し、項目名で値を取得できるようにするComponent。
class ComputeResult : public engine::Component, public ::IRenderReceiver
{
    CRS::Buffer m_compute_result_buffer_;
    // 読み戻し中のリクエスト。id == 0 なら読み戻していない
    CRS::ReadbackTicket m_ticket_;
    bool m_is_subscribed_ = false;
    std::array<uint32_t, ComputeResultTypes::kOffset> m_result_ = {};

    /// 描画のタイミング(Render)で読み戻しをリクエストするため、UpdateManagerへの登録を切り替える。
    void SetSubscribed(bool subscribed);

public:
    void OnStart() override;
    void OnEnabled() override;
    void OnDisabled() override;
    /// 読み戻しが完了していれば結果を取り込む。
    void OnUpdate() override;
    void OnDestroy() override;
    /// 読み戻し中でなければ、結果バッファの読み戻しをリクエストする(BeginFrameとEndFrameの間で呼ばれる)。
    void Render() override;

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