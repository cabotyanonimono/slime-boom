#pragma once

// 設定項目(ウィンドウモード・FPS上限・解像度・音量)を保持するデータ。
struct SettingsModel
{
    uint32_t window_mode;
    uint32_t fps_limit;
    uint32_t screen_width;
    uint32_t screen_height;
    uint32_t volume;
};
