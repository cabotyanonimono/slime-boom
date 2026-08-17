#pragma once

struct alignas(16) ExpData 
{
    Vector3 pos;
    float time;
    uint32_t is_active;
};
