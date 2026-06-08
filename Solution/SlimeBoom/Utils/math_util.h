#pragma once

namespace MathUtils
{
constexpr float DegToRad(const float deg)
{
    return deg * std::numbers::pi_v<float> / 180.0f;
}

constexpr float RadToDeg(float rad)
{
    return rad * 180.0f / std::numbers::pi_v<float>;
}

inline Quaternion LookRotationSmooth(const Vector3 &forward, const Vector3 &up, const Quaternion &current_rotation, float rotation_speed, float delta_time)
{
    using namespace DirectX;
    
    XMVECTOR f = XMLoadFloat3(&forward);
    XMVECTOR u = XMLoadFloat3(&up);
    
    if (XMVector3Less(XMVector3LengthSq(f), XMVectorSet(1e-6f, 1e-6f, 1e-6f, 1e-6f))) {
        return current_rotation;
    }
    f = XMVector3Normalize(f);
    
    XMMATRIX look_at_mat = XMMatrixLookToRH(XMVectorZero(), f, u);
    
    XMMATRIX world_rot_mat = XMMatrixTranspose(look_at_mat); 
    XMVECTOR target_q = XMQuaternionRotationMatrix(world_rot_mat);

    XMVECTOR current_q = XMLoadFloat4(&current_rotation);
    
    if (XMVectorGetX(XMQuaternionDot(current_q, target_q)) < 0.0f) {
        target_q = XMVectorNegate(target_q);
    }
    
    XMVECTOR diff_q = XMQuaternionMultiply(XMQuaternionInverse(current_q), target_q);
    
    float angle;
    XMVECTOR axis;
    XMQuaternionToAxisAngle(&axis, &angle, diff_q);
    
    if (angle < 0.001f) {
        return current_rotation;
    }
    
    float max_step = rotation_speed * delta_time;
    float t = std::min(1.0f, max_step / angle);
    
    XMVECTOR new_q = XMQuaternionSlerp(current_q, target_q, t);
    new_q = XMQuaternionNormalize(new_q);

    Quaternion result;
    XMStoreFloat4(&result, new_q);
    return result;
}

inline float YawAngle(const Vector3& v1, const Vector3& v2)
{
    Vector3 a = { v1.x, 0.0f, v1.z };
    Vector3 b = { v2.x,   0.0f, v2.z };

    a.Normalize();
    b.Normalize();

    const float angle = std::acos(std::clamp(a.Dot(b), -1.0f, 1.0f));

    return RadToDeg(angle);
}
}