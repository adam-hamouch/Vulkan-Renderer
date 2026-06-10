#include "../includes/Math.h"

void Vector3D::print() const {
    std::cout << "(" << x << ", " << y << ", " << z << ")" << std::endl;
}

Vector3D Vector3D::InverseVector() {
    return Vector3D(-x, -y, -z);
}

Vector3D Vector3D::AdditionVector(Vector3D v1, Vector3D v2) {
    return Vector3D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3D Vector3D::AddScalar(float scalar) const {
    return Vector3D(x + scalar, y + scalar, z + scalar);
}

Vector3D Vector3D::MultiplyVectors(Vector3D v) const {
    return Vector3D(x * v.x, y * v.y, z * v.z);
}

Vector3D Vector3D::MultiplyByScalar(float scalar) const {
    return Vector3D(x * scalar, y * scalar, z * scalar);
}

float Vector3D::Norm() const {
    return std::sqrt(x * x + y * y + z * z);
}

Vector3D Vector3D::Normalize() const
{
    float norm = Norm();
    if (norm != 0.0f) {
        return Vector3D(x / norm, y / norm, z / norm);
    }
    return Vector3D(0.0f, 0.0f, 0.0f);
}

Vector3D Vector3D::Normalize(Vector3D v)
{
	float norm = v.Norm();
	if (norm != 0.0f) {
		return Vector3D(v.x / norm, v.y / norm, v.z / norm);
	}
	return Vector3D(0.0f, 0.0f, 0.0f);
}

Vector3D Vector3D::operator+(const Vector3D& other) const {
    return Vector3D(x + other.x, y + other.y, z + other.z);
}

Vector3D Vector3D::operator+(float scalar) const {
    return AddScalar(scalar);
}

Vector3D Vector3D::operator-(const Vector3D& other) const {
    return Vector3D(x - other.x, y - other.y, z - other.z);
}

Vector3D Vector3D::operator*(float scalar) const {
    return MultiplyByScalar(scalar);
}

Vector3D Vector3D::operator*(const Vector3D& other) const
{
    return MultiplyVectors(other);
}

Vector3D Vector3D::operator/(float scalar) const {
    if (scalar != 0.0) {
        return Vector3D(x / scalar, y / scalar, z / scalar);
    }
    else {
        std::cerr << " Division by zero." << std::endl;
        return *this;
    }
}

Vector3D& Vector3D::operator+=(const Vector3D& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vector3D& Vector3D::operator-=(const Vector3D& other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

Vector3D& Vector3D::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}

Vector3D& Vector3D::operator*=(const Vector3D& other)
{
    x *= other.x;
    y *= other.y;
    z *= other.z;

    return *this;
}

Vector3D& Vector3D::operator/=(float scalar) {
    if (scalar != 0.0) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
    }
    else {
        std::cerr << " Division by zero." << std::endl;
    }
    return *this;
}

float Vector3D::DotProduct(const Vector3D& other) const {
    return x * other.x + y * other.y + z * other.z;
}

Vector3D Vector3D::CrossProduct(const Vector3D& other) const {
    float resultX = y * other.z - z * other.y;
    float resultY = z * other.x - x * other.z;
    float resultZ = x * other.y - y * other.x;

    return Vector3D(resultX, resultY, resultZ);
}

Vector3D Vector3D::Lerp(const Vector3D& a, const Vector3D& b, float t)
{
    return a + (b - a) * t;
}

Vector3D Vector3D::CrossProduct(Vector3D v1, Vector3D v2)
{
    float resultX = v1.y * v2.z - v1.z * v2.y;
    float resultY = v1.z * v2.x - v1.x * v2.z;
    float resultZ = v1.x * v2.y - v1.y * v2.x;

    return Vector3D(resultX, resultY, resultZ);
}