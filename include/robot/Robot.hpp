#pragma once

#include "physics/RigidBody.hpp"
#include "robot/Motor.hpp"
#include "robot/Encoder.hpp"
#include "robot/InertialSensor.hpp"
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
    std::vector<std::shared_ptr<Encoder>> encoders;
    std::shared_ptr<InertialSensor> imu;
    
    double width;
    double length;

    Robot(double w, double l, double mass);

    void addWheel(Vector2D position, double radius, double maxSpeed);
    
    void update(double dt);

    // API
    int addMotor(double maxVelocity, double maxTorque);
    void setMotorVelocity(int motorIndex, double velocity);
    double getMotorVelocity(int motorIndex);

    int addEncoder(int motorIndex);
    double getEncoderPosition(int encoderIndex);
    
    void setIMU();
    double getIMUHeading();
    double getIMURotation();
};

}