#pragma once

#include "physics/RigidBody.hpp"
#include "robot/Motor.hpp"
#include <vector>
#include <memory>

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
    std::vector<std::shared_ptr<Motor>> motors;
    double width;
    double length;

    Robot(double w, double l, double mass);

    void addWheel(Vector2D position, double radius, double maxSpeed);
    
    void update(double dt);

    // API
    int addMotor(double maxVelocity, double maxTorque);
    void setMotorVelocity(int motorIndex, double velocity);
    double getMotorVelocity(int motorIndex);
};

}
