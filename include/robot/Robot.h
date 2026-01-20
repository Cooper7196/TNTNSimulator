#ifndef ROBOT_H
#define ROBOT_H

#include "physics/RigidBody.h"
#include <vector>

struct Wheel {
    Vector2D localPosition; // Position relative to robot center
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
    
    // Future methods for sensor integration and control will go here
};

#endif // ROBOT_H
