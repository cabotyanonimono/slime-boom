#include "pch.h"
#include "gpu_buffer.h"

#include <CaboRenderingSystem/render_engine.h>
#include <Rendering/crs_bridge.h>

namespace SlimeBoom::GpuBuffer
{
namespace
{
CRS::Buffer CreateZeroInitialized(const uint64_t size, const uint32_t stride, const CRS::BufferUsage usage,
                                  const std::string &debug_name)
{
    const std::vector<uint8_t> zeros(size, 0);

    CRS::BufferDesc desc;
    desc.size = size;
    desc.stride = stride;
    desc.usage = usage;
    desc.memory = CRS::MemoryType::GpuOnly;
    desc.initial_data = zeros.data();
    desc.debug_name = debug_name;
    return CRS::Buffer(engine::CrsBridge::Crs().Device(), desc);
}
}

CRS::Buffer CreateReadWriteStructured(const uint32_t stride, const uint32_t count, const std::string &debug_name)
{
    return CreateZeroInitialized(static_cast<uint64_t>(stride) * count, stride,
                                 CRS::BufferUsage::Structured | CRS::BufferUsage::ReadWrite, debug_name);
}

CRS::Buffer CreateReadWriteRaw(const uint32_t size, const std::string &debug_name)
{
    const auto aligned_size = (static_cast<uint64_t>(size) + 3) / 4 * 4;
    return CreateZeroInitialized(aligned_size, 0, CRS::BufferUsage::Raw | CRS::BufferUsage::ReadWrite, debug_name);
}

CRS::Buffer CreateUploadStructured(const uint32_t stride, const uint32_t count, const std::string &debug_name)
{
    CRS::BufferDesc desc;
    desc.size = static_cast<uint64_t>(stride) * count;
    desc.stride = stride;
    desc.usage = CRS::BufferUsage::Structured;
    desc.memory = CRS::MemoryType::Upload;
    desc.debug_name = debug_name;
    return CRS::Buffer(engine::CrsBridge::Crs().Device(), desc);
}
}
