#include "physics/PhysicsEngine.h"

PhysicsEngine::PhysicsEngine() {}

void PhysicsEngine::addRobot(Robot* robot) {
    robots.push_back(robot);
}

void PhysicsEngine::update(double dt) {
    for (auto* robot : robots) {
        robot->body.update(dt);
    }
}
