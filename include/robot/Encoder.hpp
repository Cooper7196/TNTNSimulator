#pragma once

namespace sim {

class Encoder {
public:
    double position; // degrees
    double velocity; // rpm

    Encoder() : position(0), velocity(0) {}

    void update(double dt, double motorVelocity) {
        // motorVelocity is in radians/second
        // 1 rad/sec = 60 / (2 * PI) rpm ~= 9.549 rpm
        velocity = motorVelocity * 9.54929658551;
        
        // Position integration
        // degrees = radians * (180 / PI)
        double degPerSec = motorVelocity * (180.0 / 3.14159265359);
        position += degPerSec * dt;
    }
};

}
