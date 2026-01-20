#pragma once

#include "physics/RigidBody.hpp"
#include <vector>

namespace sim {

struct Wheel {
    Vector2D localPosition;
    double radius;
    double maxSpeed;
    
    Wheel(Vector2D pos, double r, double speed) 
        : localPosition(pos), radius(r), maxSpeed(speed) {}
};

class Robot {
public:
    RigidBody body;
    std::vector<Wheel> wheels;
    double width;
    double length;

    Robot(double w, double l, double mass);

    void addWheel(Vector2D position, double radius, double maxSpeed);
};

}