static const float4x4 bayer_matrix = float4x4(
0.0,  8.0,  2.0, 10.0,
12.0,  4.0, 14.0,  6.0,
3.0, 11.0,  1.0,  9.0,
15.0,  7.0, 13.0,  5.0
) / 16.0;

void Dithering(in uint2 screen_pos, in float3 world_pos, in float3 camera_pos, in float3 player_pos, float radius)
{
    float3 cam_to_player = player_pos - camera_pos;
    float line_length = length(cam_to_player);
    float3 line_dir = normalize(cam_to_player);

    float3 cam_to_pixel = world_pos - camera_pos;

    float t = dot(cam_to_pixel, line_dir);

    t = clamp(t, 0.0, line_length);

    float3 closest_point = camera_pos + line_dir * t;

    float distanceToLine = length(world_pos - closest_point);
    
    float alpha = distanceToLine / radius;
    
    if (distanceToLine < radius && length(cam_to_pixel) < line_length)
    {
        uint x = screen_pos.x % 4;
        uint y = screen_pos.y % 4;
        float threshold = bayer_matrix[x][y];

        clip(alpha - threshold);
    }
}