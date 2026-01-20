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

    // Update encoders (assuming 1-to-1 mapping with motors for simplicity for now)
    // In a real scenario, we'd map encoders to specific shafts/wheels
    for (int i = 0; i < encoders.size(); ++i) {
        // Just using the i-th motor for the i-th encoder as a placeholder mapping
        if (i < motors.size()) {
            encoders[i]->update(dt, motors[i]->velocity);
        }
    }

    if (imu) {
        imu->update(dt, body.angularVelocity);
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

int Robot::addEncoder(int motorIndex) {
    // For now, we store encoders in a list. 
    // Ideally, we link them to a motor or wheel.
    encoders.push_back(std::make_shared<Encoder>());
    return encoders.size() - 1;
}

double Robot::getEncoderPosition(int encoderIndex) {
    if (encoderIndex >= 0 && encoderIndex < encoders.size()) {
        return encoders[encoderIndex]->position;
    }
    return 0.0;
}

void Robot::setIMU() {
    imu = std::make_shared<InertialSensor>();
}

double Robot::getIMUHeading() {
    if (imu) return imu->heading;
    return 0.0;
}

double Robot::getIMURotation() {
    if (imu) return imu->rotation;
    return 0.0;
}

}
