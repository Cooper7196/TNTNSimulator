#pragma once

#include "Vector2D.hpp"

namespace sim {

struct RigidBody {
    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;
    
    double angle;              // radians
    double angularVelocity;    // radians/sec
    double angularAcceleration;// radians/sec^2

    double mass;               // kg
    double momentOfInertia;    // kg*m^2

    RigidBody() 
        : position(0, 0), velocity(0, 0), acceleration(0, 0),
          angle(0), angularVelocity(0), angularAcceleration(0),
          mass(1.0), momentOfInertia(1.0) {}

    void applyForce(const Vector2D& force) {
        if (mass > 0) {
            acceleration += force / mass;
        }
    }

    void applyTorque(double torque) {
        if (momentOfInertia > 0) {
            angularAcceleration += torque / momentOfInertia;
        }
    }

    void update(double dt) {
        velocity += acceleration * dt;
        position += velocity * dt;

        angularVelocity += angularAcceleration * dt;
        angle += angularVelocity * dt;

        // Reset accelerations for next step
        acceleration = Vector2D(0, 0);
        angularAcceleration = 0;
    }
};

}