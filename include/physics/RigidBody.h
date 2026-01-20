#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "Vector2D.h"

struct RigidBody {
    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;
    
    double angle;          // Orientation in radians
    double angularVelocity;
    double angularAcceleration;

    double mass;
    double momentOfInertia;

    RigidBody() 
        : position(0, 0), velocity(0, 0), acceleration(0, 0),
          angle(0), angularVelocity(0), angularAcceleration(0),
          mass(1.0), momentOfInertia(1.0) {}

    void applyForce(const Vector2D& force) {
        acceleration += force / mass;
    }

    void applyTorque(double torque) {
        angularAcceleration += torque / momentOfInertia;
    }

    void update(double dt) {
        velocity += acceleration * dt;
        position += velocity * dt;

        angularVelocity += angularAcceleration * dt;
        angle += angularVelocity * dt;

        // Reset accelerations for the next step (assuming forces are applied per step)
        acceleration = Vector2D(0, 0);
        angularAcceleration = 0;
    }
};

#endif // RIGIDBODY_H
