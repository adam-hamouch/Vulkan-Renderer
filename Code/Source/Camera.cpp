#include "Camera.h"

void Camera::SetProjectionMatrix(float fovY, float aspect, float near, float far)
{
    Mat4 projection{
        1.f / (aspect * tanf(fovY / 2.f)), 0.f, 0.f, 0.f,
        0.f, 1.f / (tanf(fovY / 2.f)), 0.f, 0.f,
        0.f, 0.f, -1.f * (far + near) / (far - near), -1.f * (2.f * far * near) / (far - near),
        0.f, 0.f, -1.f, 0.f
        };

    projectionMatrix = projection;
}

void Camera::SetOrthogonalMatrix(float left, float right, float bottom, float top, float far, float near)
{
    Mat4 orthogonal{
        2.0f / (right - left), 0.0f,                     0.0f,                    0.0f,
        0.0f,                     2.0f / (top - bottom), 0.0f,                    0.0f,
        0.0f,                     0.0f,                   -2.0f / (far - near),   0.0f,
        -(right + left) / (right - left),
        -(top + bottom) / (top - bottom),
        -(far + near) / (far - near),
        1.0f
        };

    orthogonalMatrix = orthogonal;
}

void Camera::SetViewMatrix()
{
    f = Vector3D::Normalize(center - eye);
    r = Vector3D::Normalize(Vector3D::CrossProduct(f, up));
    u = Vector3D::CrossProduct(r, f);

    viewMatrix = {
        r.x, r.y, r.z, -r.DotProduct(eye),
        u.x, u.y, u.z, -u.DotProduct(eye),
        -f.x, -f.y, -f.z, f.DotProduct(eye),
        0.0f, 0.0f, 0.0f, 1.0f
    };
}