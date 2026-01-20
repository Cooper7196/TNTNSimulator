#pragma once

#include <cmath>

namespace sim {

class Vector2D {
public:
    double x, y;

    Vector2D() : x(0.0), y(0.0) {}
    Vector2D(double nx, double ny) : x(nx), y(ny) {}

    static Vector2D fromPolar(double angle, double magnitude) {
        return Vector2D(magnitude * std::cos(angle), magnitude * std::sin(angle));
    }

    double getX() const { return x; }
    double getY() const { return y; }

    Vector2D operator+(const Vector2D& other) const { return Vector2D(x + other.x, y + other.y); }
    Vector2D operator-(const Vector2D& other) const { return Vector2D(x - other.x, y - other.y); }
    Vector2D operator*(double factor) const { return Vector2D(x * factor, y * factor); }
    Vector2D operator/(double factor) const { return Vector2D(x / factor, y / factor); }

    Vector2D& operator+=(const Vector2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    double dot(const Vector2D& other) const { return x * other.x + y * other.y; }
    double cross(const Vector2D& other) const { return x * other.y - y * other.x; }

    double theta() const { return std::atan2(y, x); }
    double magnitude() const { return std::sqrt(x * x + y * y); }

    Vector2D normalize() const {
        double m = magnitude();
        if (m == 0) return Vector2D(0, 0);
        return Vector2D(x / m, y / m);
    }

    void rotateBy(double angle) {
        double m = magnitude();
        double t = theta() + angle;
        x = m * std::cos(t);
        y = m * std::sin(t);
    }
};

}