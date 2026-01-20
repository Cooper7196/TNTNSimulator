#pragma once

namespace sim {

class Motor {
public:
    double velocity; // radians/second
    double targetVelocity;
    double maxVelocity;
    double maxTorque;

    Motor(double maxVel, double maxTorq) 
        : velocity(0), targetVelocity(0), maxVelocity(maxVel), maxTorque(maxTorq) {}

    void setVelocity(double vel) {
        if (vel > maxVelocity) targetVelocity = maxVelocity;
        else if (vel < -maxVelocity) targetVelocity = -maxVelocity;
        else targetVelocity = vel;
    }

    // Simple motor model: instantly achieve target velocity for now, 
    // or we could model acceleration based on torque/load.
    // For this step, we'll keep it simple but extensible.
    void update(double dt) {
        // Simple P-controller like behavior or instantaneous
        velocity = targetVelocity; 
    }
};

}
