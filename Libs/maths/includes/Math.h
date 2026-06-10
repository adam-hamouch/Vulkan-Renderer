#pragma once
#include <vector>
#include <iostream>
#include <cmath>

#define PI 3.14159265358979323846f

inline float DegToRad(float degrees) {
    return degrees * (PI / 180.0f);
}

inline float RadToDeg(float radians) {
	return radians * (180.0f / PI);
}

class  Vector2D {
public:
    float x, y;

    Vector2D() = default; 
    Vector2D(float _x, float _y) : x(_x), y(_y) {}

    void print() const;
    Vector2D InverseVector();
    Vector2D AdditionVector(Vector2D v1, Vector2D v2);
    Vector2D AddScalar(float scalar) const;
    Vector2D MultiplyVectors(Vector2D v) const;
    Vector2D MultiplyByScalar(float scalar) const;
    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator+(float scalar) const;
    Vector2D operator-(const Vector2D& other) const;
    Vector2D operator*(float scalar) const;
    Vector2D operator*(const Vector2D& other) const;
    Vector2D operator/(float scalar) const;
    Vector2D& operator+=(const Vector2D& other);
    Vector2D& operator-=(const Vector2D& other);
    Vector2D& operator*=(float scalar);
    Vector2D& operator*=(const Vector2D& other);
    Vector2D& operator/=(float scalar);
    float Norm() const;
    float DotProduct(const Vector2D& v1, const Vector2D& v2) const;
};

class  Vector3D {
public:
    float x, y, z;
    Vector3D() = default; 
    Vector3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
    {
    }

    void print() const;
    Vector3D InverseVector();
    Vector3D AdditionVector(Vector3D v1, Vector3D v2);
    Vector3D AddScalar(float scalar) const;
    Vector3D MultiplyVectors(Vector3D v) const;
    Vector3D MultiplyByScalar(float scalar) const;
    Vector3D operator+(const Vector3D& other) const;
    Vector3D operator+(float scalar) const;
    Vector3D operator-(const Vector3D& other) const;
    Vector3D operator*(float scalar) const;
    Vector3D operator*(const Vector3D& other) const;
    Vector3D operator/(float scalar) const;
    Vector3D& operator+=(const Vector3D& other);
    Vector3D& operator-=(const Vector3D& other);
    Vector3D& operator*=(float scalar);
    Vector3D& operator*=(const Vector3D& other);
    Vector3D& operator/=(float scalar);
    float Norm() const;
	Vector3D Normalize() const;
    float DotProduct(const Vector3D& other) const;
    Vector3D CrossProduct(const Vector3D& other) const;
    static Vector3D Lerp(const Vector3D& a, const Vector3D& b, float t);

    static Vector3D Normalize(Vector3D v);
    static Vector3D CrossProduct(Vector3D v1, Vector3D v2); 
};

class  Quaternion
{
public:
    float x, y, z, w;

    Quaternion() = default;
    Quaternion(float _x, float _y, float _z, float _w) : x(_x), y(_x), z(_z), w(_w) {}

    static Quaternion FromEuler(const Vector3D& euler)
    {
        float cx = cosf(euler.x * 0.5f);
        float sx = sinf(euler.x * 0.5f);
        float cy = cosf(euler.y * 0.5f);
        float sy = sinf(euler.y * 0.5f);
        float cz = cosf(euler.z * 0.5f);
        float sz = sinf(euler.z * 0.5f);

        Quaternion q;
        q.w = cx * cy * cz + sx * sy * sz;
        q.x = sx * cy * cz - cx * sy * sz;
        q.y = cx * sy * cz + sx * cy * sz;
        q.z = cx * cy * sz - sx * sy * cz;
        return q;
    }

    static Quaternion FromAxisAngle(const Vector3D& axis, float angleRad)
    {
        float halfAngle = angleRad * 0.5f;
        float s = sinf(halfAngle);

        Vector3D normAxis = axis.Normalize();

        Quaternion q;
        q.w = cosf(halfAngle);
        q.x = normAxis.x * s;
        q.y = normAxis.y * s;
        q.z = normAxis.z * s;
        return q;
    }

    Vector3D ToEulerAngles() const
    {
        Vector3D euler;

        float sinr_cosp = 2.0f * (w * x + y * z);
        float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
        euler.x = std::atan2(sinr_cosp, cosr_cosp);

        float sinp = 2.0f * (w * y - z * x);
        if (fabs(sinp) >= 1.0f)
            euler.y = std::copysign(PI / 2.0f, sinp); 
        else
            euler.y = std::asin(sinp);

        float siny_cosp = 2.0f * (w * z + x * y);
        float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
        euler.z = std::atan2(siny_cosp, cosy_cosp);

        return euler;
    }

    void Normalize()
    {
        float mag = sqrtf(x * x + y * y + z * z + w * w);
        if (mag > 0.0f)
        {
            x /= mag;
            y /= mag;
            z /= mag;
            w /= mag;
        }
    }

    Quaternion operator*(const Quaternion& q) const
    {
        Quaternion result;

        result.w = w * q.w - x * q.x - y * q.y - z * q.z;
        result.x = w * q.x + x * q.w + y * q.z - z * q.y;
        result.y = w * q.y - x * q.z + y * q.w + z * q.x;
        result.z = w * q.z + x * q.y - y * q.x + z * q.w;

        return result;
    }

};

class  Vector4D
{
public:
    float x = 0, y = 0, z = 0, w = 0;

    Vector4D() = default;
    //Vector4D(float a, float b, float c, float d);
    //Vector4D(const Vector3D& v, float d = 0);

    Vector4D(float a, float b, float c, float d)
    {
        x = a;
        y = b;
        z = c;
        w = d;
    }

    Vector4D(const Vector3D& v, float d)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        w = d;
    }

    Vector4D& operator/=(const float& a)
    {
        x /= a;
        y /= a;
        z /= a;
        w /= a;
        return *this;
    }

    ~Vector4D() = default;

    //Vector4D& operator/=(const float& a); 
};

class  Transform
{
public:
    Transform() = default; 
    Transform(Vector3D _pos, Vector3D _eulerRot, Vector3D _scale); 
    ~Transform() = default; 

    Vector3D pos;
    Vector3D eulerRot;
    Vector3D scale;
};

class  Mat4 {
public:
    Mat4(float e00, float e01, float e02, float e03,
        float e10, float e11, float e12, float e13,
        float e20, float e21, float e22, float e23,
        float e30, float e31, float e32, float e33)
    {
        data[0] = e00;  data[1] = e01;  data[2] = e02;  data[3] = e03;
        data[4] = e10;  data[5] = e11;  data[6] = e12;  data[7] = e13;
        data[8] = e20;  data[9] = e21;  data[10] = e22; data[11] = e23;
        data[12] = e30; data[13] = e31; data[14] = e32; data[15] = e33;
    }

    Mat4() {
        for (int i = 0; i < 16; i++)
            data[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    }

    float data[16];
    bool isOrthogonal = false;

    void Print();
    void IsOrthogonal();
    static Mat4 Identity();
    static Mat4 SetTranslation(const Vector3D& position);
    static Mat4 SetScale(const Vector3D& position);
    Mat4 MultiplyMatrices(const Mat4& other);
    static Mat4 RotationX(float angle);
    static Mat4 RotationY(float angle);
    static Mat4 RotationZ(float angle);
    Mat4 InvertMatrix();
    Mat4 Transpose() const;
    float* TransposeData() const;
    static Mat4 TRS(Vector3D pos, Vector3D rot, Vector3D scale);
    static Vector3D TransformDirection(const Mat4& matrix, const Vector3D& direction); 

    Mat4 operator*(const Mat4& other) const;
    Vector3D operator*(const Vector3D& other) const;
    Vector4D operator*(const Vector4D& v) const;
};