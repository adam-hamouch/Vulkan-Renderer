#include "../includes/Math.h"

void Mat4::Print()
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << data[i * 4 + j] << "\t";
        }
        std::cout << std::endl;
    }
}

Mat4 Mat4::Identity() {
    return Mat4(1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}

Mat4 Mat4::MultiplyMatrices(const Mat4& other)
{
    float result[16];
    Mat4 mat = mat.Identity();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result[i * 4 + j] =
                data[i * 4 + 0] * other.data[0 * 4 + j] +
                data[i * 4 + 1] * other.data[1 * 4 + j] +
                data[i * 4 + 2] * other.data[2 * 4 + j] +
                data[i * 4 + 3] * other.data[3 * 4 + j];
        }
    }

    for (int i = 0; i < 16; ++i) {
        mat.data[i] = result[i];
    }

    return mat;
}


Mat4 Mat4::SetTranslation(const Vector3D& other)
{
    Mat4 translation = translation.Identity();

    translation.data[3] = other.x;
    translation.data[7] = other.y;
    translation.data[11] = other.z;

    return translation;
}

Mat4 Mat4::SetScale(const Vector3D& other)
{
    Mat4 scale = scale.Identity();

    scale.data[0] *= other.x;
    scale.data[5] *= other.y;
    scale.data[10] *= other.z;

    return scale;
}

Mat4 Mat4::RotationX(float angle)
{
    return Mat4(1, 0, 0, 0,
        0, cos(angle), -sin(angle), 0,
        0, sin(angle), cos(angle), 0,
        0, 0, 0, 1);
}

Mat4 Mat4::RotationY(float angle)
{
    return Mat4(cos(angle), 0, sin(angle), 0,
        0, 1, 0, 0,
        -sin(angle), 0, cos(angle), 0,
        0, 0, 0, 1);
}

Mat4 Mat4::RotationZ(float angle)
{
    return Mat4(cos(angle), -sin(angle), 0, 0,
        sin(angle), cos(angle), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
}

Mat4 Mat4::InvertMatrix()
{
    Mat4 result = result.Identity();
    float inv[16], det;
    int i;

    inv[0] = data[5] * data[10] * data[15] -
        data[5] * data[11] * data[14] -
        data[9] * data[6] * data[15] +
        data[9] * data[7] * data[14] +
        data[13] * data[6] * data[11] -
        data[13] * data[7] * data[10];

    inv[4] = -data[4] * data[10] * data[15] +
        data[4] * data[11] * data[14] +
        data[8] * data[6] * data[15] -
        data[8] * data[7] * data[14] -
        data[12] * data[6] * data[11] +
        data[12] * data[7] * data[10];

    inv[8] = data[4] * data[9] * data[15] -
        data[4] * data[11] * data[13] -
        data[8] * data[5] * data[15] +
        data[8] * data[7] * data[13] +
        data[12] * data[5] * data[11] -
        data[12] * data[7] * data[9];

    inv[12] = -data[4] * data[9] * data[14] +
        data[4] * data[10] * data[13] +
        data[8] * data[5] * data[14] -
        data[8] * data[6] * data[13] -
        data[12] * data[5] * data[10] +
        data[12] * data[6] * data[9];

    inv[1] = -data[1] * data[10] * data[15] +
        data[1] * data[11] * data[14] +
        data[9] * data[2] * data[15] -
        data[9] * data[3] * data[14] -
        data[13] * data[2] * data[11] +
        data[13] * data[3] * data[10];

    inv[5] = data[0] * data[10] * data[15] -
        data[0] * data[11] * data[14] -
        data[8] * data[2] * data[15] +
        data[8] * data[3] * data[14] +
        data[12] * data[2] * data[11] -
        data[12] * data[3] * data[10];

    inv[9] = -data[0] * data[9] * data[15] +
        data[0] * data[11] * data[13] +
        data[8] * data[1] * data[15] -
        data[8] * data[3] * data[13] -
        data[12] * data[1] * data[11] +
        data[12] * data[3] * data[9];

    inv[13] = data[0] * data[9] * data[14] -
        data[0] * data[10] * data[13] -
        data[8] * data[1] * data[14] +
        data[8] * data[2] * data[13] +
        data[12] * data[1] * data[10] -
        data[12] * data[2] * data[9];

    inv[2] = data[1] * data[6] * data[15] -
        data[1] * data[7] * data[14] -
        data[5] * data[2] * data[15] +
        data[5] * data[3] * data[14] +
        data[13] * data[2] * data[7] -
        data[13] * data[3] * data[6];

    inv[6] = -data[0] * data[6] * data[15] +
        data[0] * data[7] * data[14] +
        data[4] * data[2] * data[15] -
        data[4] * data[3] * data[14] -
        data[12] * data[2] * data[7] +
        data[12] * data[3] * data[6];

    inv[10] = data[0] * data[5] * data[15] -
        data[0] * data[7] * data[13] -
        data[4] * data[1] * data[15] +
        data[4] * data[3] * data[13] +
        data[12] * data[1] * data[7] -
        data[12] * data[3] * data[5];

    inv[14] = -data[0] * data[5] * data[14] +
        data[0] * data[6] * data[13] +
        data[4] * data[1] * data[14] -
        data[4] * data[2] * data[13] -
        data[12] * data[1] * data[6] +
        data[12] * data[2] * data[5];

    inv[3] = -data[1] * data[6] * data[11] +
        data[1] * data[7] * data[10] +
        data[5] * data[2] * data[11] -
        data[5] * data[3] * data[10] -
        data[9] * data[2] * data[7] +
        data[9] * data[3] * data[6];

    inv[7] = data[0] * data[6] * data[11] -
        data[0] * data[7] * data[10] -
        data[4] * data[2] * data[11] +
        data[4] * data[3] * data[10] +
        data[8] * data[2] * data[7] -
        data[8] * data[3] * data[6];

    inv[11] = -data[0] * data[5] * data[11] +
        data[0] * data[7] * data[9] +
        data[4] * data[1] * data[11] -
        data[4] * data[3] * data[9] -
        data[8] * data[1] * data[7] +
        data[8] * data[3] * data[5];

    inv[15] = data[0] * data[5] * data[10] -
        data[0] * data[6] * data[9] -
        data[4] * data[1] * data[10] +
        data[4] * data[2] * data[9] +
        data[8] * data[1] * data[6] -
        data[8] * data[2] * data[5];

    
    det = data[0] * inv[0] + data[1] * inv[4] + data[2] * inv[8] + data[3] * inv[12];

    if (det == 0) {
        std::cout << det << std::endl;
        return Identity();
    }

    det = 1.0f / det;

    for (i = 0; i < 16; i++)
        result.data[i] = inv[i] * det;

    return result;
}

Mat4 Mat4::operator*(const Mat4& other) const
{
    Mat4 result;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.data[i * 4 + j] =
                data[i * 4 + 0] * other.data[0 * 4 + j] +
                data[i * 4 + 1] * other.data[1 * 4 + j] +
                data[i * 4 + 2] * other.data[2 * 4 + j] +
                data[i * 4 + 3] * other.data[3 * 4 + j];
        }
    }

    return result;
}
Vector3D Mat4::operator*(const Vector3D& other) const
{
    float x = data[0] * other.x + data[1] * other.y + data[2] * other.z + data[3] * 1;
    float y = data[4] * other.x + data[5] * other.y + data[6] * other.z + data[7] * 1;
    float z = data[8] * other.x + data[9] * other.y + data[10] * other.z + data[11] * 1;
    float w = data[12] * other.x + data[13] * other.y + data[14] * other.z + data[15] * 1;

    if (w != 0.0f) {
        x /= w;
        y /= w;
        z /= w;
    }

    return Vector3D(x, y, z);
}

Vector4D Mat4::operator*(const Vector4D& v) const
{
    float x = data[0] * v.x + data[4] * v.y + data[8] * v.z + data[12] * v.w;
    float y = data[1] * v.x + data[5] * v.y + data[9] * v.z + data[13] * v.w; 
    float z = data[2] * v.x + data[6] * v.y + data[10] * v.z + data[14] * v.w;
    float w = data[3] * v.x + data[7] * v.y + data[11] * v.z + data[15] * v.w; 

    if (w != 0.0f) {
        x /= w;
        y /= w;
        z /= w;
    }

    return Vector4D(x, y, z, w); 
}

Mat4 Mat4::Transpose() const {
    return Mat4(
        data[0], data[4], data[8], data[12],
        data[1], data[5], data[9], data[13],
        data[2], data[6], data[10], data[14],
        data[3], data[7], data[11], data[15]
    );
}

float* Mat4::TransposeData() const
{
    float* transposed = new float[16] {
        data[0], data[4], data[8], data[12],
            data[1], data[5], data[9], data[13],
            data[2], data[6], data[10], data[14],
            data[3], data[7], data[11], data[15]
        };
    return transposed;
}

Mat4 Mat4::TRS(Vector3D pos, Vector3D rot, Vector3D scale)
{
    return SetTranslation(pos) * (RotationX(rot.x) * RotationY(rot.y) * RotationZ(rot.z)) * SetScale(scale);
}

Vector3D Mat4::TransformDirection(const Mat4& matrix, const Vector3D& direction)
{
    Vector3D result;
    result.x = matrix.data[0] * direction.x + matrix.data[4] * direction.y + matrix.data[8] * direction.z;
    result.y = matrix.data[1] * direction.x + matrix.data[5] * direction.y + matrix.data[9] * direction.z;
    result.z = matrix.data[2] * direction.x + matrix.data[6] * direction.y + matrix.data[10] * direction.z;
    return result;
}

void Mat4::IsOrthogonal()
{
    float epsilon = 1e-5f;
    Mat4* mat = this;


    Mat4 transpose = mat->Transpose();
    Mat4 product = mat->MultiplyMatrices(transpose);

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            float expected = (i == j) ? 1.0f : 0.0f;
            if (std::fabs(product.data[i * 4 + j] - expected) > epsilon) {
                isOrthogonal = false;
                return;
            }
        }
    }
    isOrthogonal = true;
}