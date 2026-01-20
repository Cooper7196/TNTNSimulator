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

}
