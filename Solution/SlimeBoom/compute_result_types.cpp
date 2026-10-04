#include "pch.h"
#include "compute_result_types.h"

size_t ComputeResultTypes::GetComputeResultBufferSize()
{
    size_t result = 0;
    for (auto &[buff_name, size] : kResultTypeOffsets)
    {
        result += size;
    }

    return result;
}

size_t ComputeResultTypes::GetComputeResultOffset(const std::string& name)
{
    size_t result = 0;
    for (auto &[buff_name, size] : kResultTypeOffsets)
    {
        if (buff_name == name)
            break;
        
        result += size;
    }

    return result;
}
