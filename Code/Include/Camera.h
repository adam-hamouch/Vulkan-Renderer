#pragma once

#include "../../Libs/maths/includes/Math.h"

class Camera
{
public:
    void SetViewMatrix();
    void SetProjectionMatrix(float fovY, float aspect, float near, float far);
    void SetOrthogonalMatrix(float left, float right, float bottom, float top, float far, float near);

    Mat4 GetViewMatrix() const { return viewMatrix; }
    Mat4 GetProjectionMatrix() const { return projectionMatrix; }
    Mat4 GetOrthogonalMatrix() const { return orthogonalMatrix; }

    Vector3D eye = { 0.f, 0.f, 3.0f };
    Vector3D center = { 0.f, 0.f, -2.f };
    Vector3D up = { 0.f, 1.f, 0.f };
    Vector3D offset = { 0.0f, 1.8f, 0.0f };
    Vector3D forward;
    Vector3D f, r, u;
    
private :
    Mat4 viewMatrix = Mat4::Identity();
    Mat4 projectionMatrix = Mat4::Identity();
    Mat4 orthogonalMatrix = Mat4::Identity();
    Mat4 viewProjectionMatrix = Mat4::Identity();
};
