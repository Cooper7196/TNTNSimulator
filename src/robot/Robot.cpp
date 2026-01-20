#include "robot/Robot.hpp"
#include <cmath>

namespace sim {

Robot::Robot(double w, double l, double mass) : width(w), length(l) {
    body.mass = mass;
    // Approximate moment of inertia for a rectangular box: I = m * (w^2 + h^2) / 12
    body.momentOfInertia = (mass * (w * w + l * l)) / 12.0;
}

void Robot::addWheel(Vector2D position, double radius, double maxSpeed) {
    wheels.push_back(Wheel(position, radius, maxSpeed));
}

void Robot::update(double dt) {
    for (auto& motor : motors) {
        motor->update(dt);
    }
}

int Robot::addMotor(double maxVelocity, double maxTorque) {
    motors.push_back(std::make_shared<Motor>(maxVelocity, maxTorque));
    return motors.size() - 1;
}

void Robot::setMotorVelocity(int motorIndex, double velocity) {
    if (motorIndex >= 0 && motorIndex < motors.size()) {
        motors[motorIndex]->setVelocity(velocity);
    }
}

double Robot::getMotorVelocity(int motorIndex) {
    if (motorIndex >= 0 && motorIndex < motors.size()) {
        return motors[motorIndex]->velocity;
    }
    return 0.0;
}

}