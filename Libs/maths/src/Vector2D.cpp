#include "../includes/Math.h"

void Vector2D::print() const {
    std::cout << "(" << x << ", " << y << ")" << std::endl;
}

Vector2D Vector2D::InverseVector() {
    return Vector2D(-x, -y);
}

Vector2D Vector2D::AdditionVector(Vector2D v1, Vector2D v2) {
    return Vector2D(v1.x + v2.x, v1.y + v2.y);
}

Vector2D Vector2D::AddScalar(float scalar) const {
    return Vector2D(x + scalar, y + scalar);
}

Vector2D Vector2D::MultiplyVectors(Vector2D v) const {
    return Vector2D(x * v.x, y * v.y);
}

Vector2D Vector2D::MultiplyByScalar(float scalar) const {
    return Vector2D(x * scalar, y * scalar);
}

Vector2D Vector2D::operator+(const Vector2D& other) const {
    return Vector2D(x + other.x, y + other.y);
}

Vector2D Vector2D::operator+(float scalar) const {
    return AddScalar(scalar);
}

Vector2D Vector2D::operator-(const Vector2D& other) const {
    return Vector2D(x - other.x, y - other.y);
}

Vector2D Vector2D::operator*(float scalar) const {
    return MultiplyByScalar(scalar);
}

Vector2D Vector2D::operator*(const Vector2D& other) const
{
    return MultiplyVectors(other);
}

Vector2D Vector2D::operator/(float scalar) const {
    if (scalar != 0.0) {
        return Vector2D(x / scalar, y / scalar);
    }
    else {
        std::cout << " Division by zero." << std::endl;
        return *this;
    }
}

Vector2D& Vector2D::operator+=(const Vector2D& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector2D& Vector2D::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2D& Vector2D::operator*=(const Vector2D& other)
{
    x *= other.x;
    y *= other.y;
    return *this;
}

Vector2D& Vector2D::operator/=(float scalar) {
    if (scalar != 0.0) {
        x /= scalar;
        y /= scalar;
    }
    else {
        std::cout << "can't divide by zero" << std::endl;
    }
    return *this;
}

float Vector2D::Norm() const {
    return std::sqrt(x * x + y * y);
}

float Vector2D::DotProduct(const Vector2D& v1, const Vector2D& v2) const {
    return v1.x * v2.x + v1.y * v2.y;
}