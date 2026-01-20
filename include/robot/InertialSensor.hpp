#pragma once

namespace sim {

class InertialSensor {
public:
    double heading; // degrees (0-360)
    double rotation; // degrees (cumulative)

    InertialSensor() : heading(0), rotation(0) {}

    void update(double dt, double angularVelocity) {
        // angularVelocity is in radians/second
        double degPerSec = angularVelocity * (180.0 / 3.14159265359);
        
        rotation += degPerSec * dt;
        
        heading = std::fmod(rotation, 360.0);
        if (heading < 0) heading += 360.0;
    }
};

}
