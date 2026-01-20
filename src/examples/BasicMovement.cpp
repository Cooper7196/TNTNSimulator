#include "robot/Robot.hpp"
#include <iostream>

using namespace sim;

void runBasicMovement(Robot& robot) {
    // Example: Move forward for 2 seconds
    
    // Add two motors (left and right)
    int leftMotor = robot.addMotor(10.0, 2.0); // max vel 10 rad/s, max torque 2 Nm
    int rightMotor = robot.addMotor(10.0, 2.0);

    // Set IMU
    robot.setIMU();

    // Set velocity to move forward
    robot.setMotorVelocity(leftMotor, 5.0);
    robot.setMotorVelocity(rightMotor, 5.0);

    std::cout << "Starting movement..." << std::endl;
}

void updateAlgorithm(Robot& robot, double time) {
    // Simple logic: stop after 2 seconds
    if (time > 2.0) {
        robot.setMotorVelocity(0, 0.0);
        robot.setMotorVelocity(1, 0.0);
    }
}
