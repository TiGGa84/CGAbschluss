#include "vector.h"

#include <cmath>
#include <assert.h>

#define EPSILON 1e-6


Vector::Vector(float x, float y, float z) : X(x), Y(y), Z(z) {
}

Vector::Vector() {
}

float Vector::dot(const Vector& v) const {
    return X * v.X + Y * v.Y + Z * v.Z;
}

Vector Vector::cross(const Vector& v) const {
    Vector c;
    c.X = Y * v.Z - Z * v.Y;
    c.Y = Z * v.X - X * v.Z;
    c.Z = X * v.Y - Y * v.X;
    return c;
}

Vector Vector::operator+(const Vector& v) const {
    return Vector(X + v.X, Y + v.Y, Z + v.Z);
}

Vector Vector::operator-(const Vector& v) const {
    return Vector(X - v.X, Y - v.Y, Z - v.Z);
}

Vector Vector::operator*(float c) const {
    return Vector(X * c, Y * c, Z * c);
}

Vector Vector::operator-() const {
    return *this * -1.0f;
}

Vector& Vector::operator+=(const Vector& v) {
    X = X + v.X;
    Y = Y + v.Y;
    Z = Z + v.Z;
    return *this;
}

Vector& Vector::normalize() {
    float len = length();
    X = X / len;
    Y = Y / len;
    Z = Z / len;
    return *this;
}

float Vector::length() const {
    return std::sqrt(lengthSquared());
}

float Vector::lengthSquared() const {
    return X * X + Y * Y + Z * Z;
}

Vector Vector::reflection(const Vector& normal) const {
    return *this -normal * (this->dot(normal) * 2.0f);
}

float triangleArea(Vector u, Vector v, Vector w) {
    return (v - u).cross(w - v).length() / 2;
}

bool Vector::triangleIntersection(const Vector& dir, const Vector& a, const Vector& b,
                                  const Vector& c, float& s) const {
    // Triangle normal
    Vector n = (b - a).cross(c - a).normalize();
    float nd = dir.dot(n);
    // Triangle  parallel to ray
    if (std::abs(nd) < EPSILON) return false;
    // Distance
    s = (n.dot(a) - n.dot(*this)) / nd;
    // Triangle behind ray
    if (s < 0) return false;
    // Intersection point
    Vector p = *this +dir * s;

    float areaSum = triangleArea(a, b, p) + triangleArea(a, c, p) + triangleArea(b, c, p);
    float area = triangleArea(a, b, c);

    if (area + EPSILON >= areaSum) return true;

    return false;
}

Vector Vector::planeIntersection(const Vector& dir, const Vector& center, const Vector& n) const {
    
    float denom = n.dot(dir);
    
    if (std::abs(denom) > EPSILON) {
        float s = (center - *this).dot(n) / denom;
        
        return *this + dir * s;
    }
    return Vector(0,0,0);
}
