#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>

struct Vector2D {
    double x;
    double y;

    Vector2D(double x = 0, double y = 0) : x(x), y(y) {}

    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }

    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }

    Vector2D operator*(double scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }

    Vector2D operator/(double scalar) const {
        return Vector2D(x / scalar, y / scalar);
    }

    Vector2D& operator+=(const Vector2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    double magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    Vector2D normalize() const {
        double mag = magnitude();
        if (mag == 0) return Vector2D(0, 0);
        return *this / mag;
    }

    double dot(const Vector2D& other) const {
        return x * other.x + y * other.y;
    }

    double cross(const Vector2D& other) const {
        return x * other.y - y * other.x;
    }
};

#endif // VECTOR2D_H
