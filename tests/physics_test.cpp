#include <iostream>
#include <cmath>
#include "physics/PhysicsEngine.hpp"
#include "robot/Robot.hpp"

using namespace sim;

int main() {
    // 1. Initialize Robot and Physics Engine
    PhysicsEngine physics;
    
    // Robot Parameters (same as main.cpp)
    Vector2D startPos(0.0, 0.0);
    double startTheta = 0.0;
    double wheelRadius = 0.0508; 
    double trackRadius = 0.1524; 
    double cartridge = 600.0; 
    double gearRatio = 1.0; 
    double mass = 4.53592; 
    double inertia = 1.0; 

    Robot robot(startPos, startTheta, wheelRadius, trackRadius, cartridge, gearRatio, mass, inertia);
    physics.addRobot(&robot);

    // 2. Set Input (Move forward)
    robot.setVoltages(6.0, 6.0); 

    // 3. Simulate for 1 second
    double dt = 0.01; // 10ms
    double totalTime = 1.0;
    int steps = (int)(totalTime / dt);

    std::cout << "Starting Simulation..." << std::endl;
    std::cout << "Initial Pos: (" << robot.getPos().getX() << ", " << robot.getPos().getY() << ")" << std::endl;

    for (int i = 0; i < steps; ++i) {
        physics.update(dt);
        
        // Print state every 10 steps
        if (i % 10 == 0) {
             std::cout << "T=" << i * dt << "s, Pos: (" << robot.getPos().getX() << ", " << robot.getPos().getY() << "), Vel: (" << robot.getVel().getX() << ", " << robot.getVel().getY() << ")" << std::endl;
        }
    }

    std::cout << "Final Pos: (" << robot.getPos().getX() << ", " << robot.getPos().getY() << ")" << std::endl;

    // 4. Verification Logic
    // We expect the robot to have moved in positive X direction (assuming 0 angle points along X)
    if (robot.getPos().getX() > 0.1) {
        std::cout << "TEST PASSED: Robot moved forward." << std::endl;
        return 0;
    } else {
        std::cout << "TEST FAILED: Robot did not move significantly." << std::endl;
        return 1;
    }
}
