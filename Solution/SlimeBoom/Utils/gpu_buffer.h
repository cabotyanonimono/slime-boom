#pragma once
#include <CaboRenderingSystem/RHI/gpu_resources.h>

namespace SlimeBoom::GpuBuffer
{
/// ComputeShaderから読み書きする(RWStructuredBuffer)、0で初期化したバッファを作る。
/// シェーダーからStructuredBufferとして読むこともできる。
/// @param stride 1要素のバイト数
/// @param count 要素数
CRS::Buffer CreateReadWriteStructured(uint32_t stride, uint32_t count, const std::string &debug_name);

/// ComputeShaderから読み書きする(RWByteAddressBuffer)、0で初期化したバッファを作る。
/// @param size バイト数(4の倍数に切り上げる)
CRS::Buffer CreateReadWriteRaw(uint32_t size, const std::string &debug_name);

/// CPUから書き込み、シェーダーからStructuredBufferとして読むバッファを作る。
/// @param stride 1要素のバイト数
/// @param count 要素数
CRS::Buffer CreateUploadStructured(uint32_t stride, uint32_t count, const std::string &debug_name);
}
