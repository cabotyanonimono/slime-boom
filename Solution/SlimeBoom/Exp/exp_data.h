#pragma once

// 経験値オーブ1個分のGPU用データ。StructuredBufferに格納するため16バイトでアラインされている。
struct alignas(16) ExpData 
{
    Vector3 pos;
    float time;
    uint32_t is_active;
};
