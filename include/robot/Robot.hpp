#pragma once

#include "physics/Vector2D.hpp"
#include "physics/Matrix.hpp"
#include <vector>

namespace sim {

class Robot {
public:
    // Physical Properties (SI Units: meters, kg, seconds, radians)
    double wheel_radius;
    double track_radius;
    double mass;
    double inertia;
    double gear_ratio;
    double cartridge_rpm; // Motor cartridge (e.g. 600, 200)

    // State
    Vector2D pos;
    Vector2D vel;
    double theta;
    
    // Motor voltages (Volts)
    double lV = 0.0;
    double rV = 0.0;

    // State-Space Matrices (2x2)
    algebra::Matrix<double, 2, 2> A_l, B_l, A_r, B_r;
    
    // State Vectors (2x1) [current_draw, velocity]? 
    // Wait, looking at lemlib again, X_l seems to be [current, angular_velocity] or similar?
    // Let's re-examine the C1/C2 constants derivation.
    // M*a = F ... I*alpha = ...
    // The state space derived usually relates Voltage to Velocity/Current.
    // For a DC motor: V = I*R + Kw*w -> I = (V - Kw*w)/R
    // Torque T = Kt*I
    // This leads to w_dot = ... which is a first order system if we ignore inductance.
    // The lemlib code has a 2x2 matrix for EACH side. 
    // Let's stick to the matrix structure from lemlib but use doubles.
    
    algebra::Vector2d X_l, X_r; 

    // Constants
    double C1_l, C2_l, C1_r, C2_r;
    double D1, D2;

    Robot(Vector2D start, double start_theta, double wheel_r, double track_r, 
          double cartridge_rpm, double gear_r, double m, double i);

    void setVoltages(double left, double right);
    void update(double dt); // dt in seconds

    // Getters
    Vector2D getPos() const { return pos; }
    double getTheta() const { return theta; }
    Vector2D getVel() const { return vel; }
};

}