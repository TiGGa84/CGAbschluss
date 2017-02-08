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
	return *this - normal * (this->dot(normal) * 2.0f);
}
