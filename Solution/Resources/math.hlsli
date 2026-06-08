#ifndef __MATH_HLSLI__
#define __MATH_HLSLI__

#define PI 3.14159265
#define PI2 6.283185307
#define DEG2RAD 0.0174532925
#define RAD2DEG 57.2957795

float3x3 EulerToRotationMatrix(float3 euler)
{
    float3 rad = euler * DEG2RAD;

    float sin_x, cos_x, sin_y, cos_y, sin_z, cos_z;
    sincos(rad.x, sin_x, cos_x);
    sincos(rad.y, sin_y, cos_y);
    sincos(rad.z, sin_z, cos_z);

    float3x3 rotation;
    
    rotation[0] = float3(cos_z * cos_y + sin_z * sin_x * sin_y, -sin_z * cos_y + cos_z * sin_x * sin_y, cos_x * sin_y);
    rotation[1] = float3(sin_z * cos_x, cos_z * cos_x, -sin_x);
    rotation[2] = float3(-cos_z * sin_y + sin_z * sin_x * cos_y, sin_z * sin_y + cos_z * sin_x * cos_y, cos_x * cos_y);

    return rotation;
}

float rand(float2 seed)
{
	return frac(sin(dot(seed, float2(12.9898, 78.233))) * 43758.5453);
}

#endif