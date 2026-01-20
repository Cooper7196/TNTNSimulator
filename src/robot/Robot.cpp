#include "robot/Robot.hpp"
#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace sim {

// Constants for V5 Motor
constexpr double STALL_TORQUE = 2.1; // Nm
constexpr double STALL_CURRENT = 2.5; // Amp
constexpr double FREE_CURRENT = 0.1; // Amp
constexpr double VOLTAGE_MAX = 12.0; // Volt

// Helper for discretization
template<int N>
std::pair<algebra::Matrix<double, N, N>, algebra::Matrix<double, N, N>> 
to_discrete(const algebra::Matrix<double, N, N>& A, const algebra::Matrix<double, N, N>& B, double dt) {
    // A_d = I + A * dt
    algebra::Matrix<double, N, N> Ad;
    for(int r=0; r<N; ++r) {
        for(int c=0; c<N; ++c) {
            Ad(r,c) = A(r,c) * dt;
        }
        Ad(r,r) += 1.0; // Add Identity
    }
    
    // B_d = B * dt
    algebra::Matrix<double, N, N> Bd;
    for(int r=0; r<N; ++r) {
        for(int c=0; c<N; ++c) {
            Bd(r,c) = B(r,c) * dt;
        }
    }

    return {Ad, Bd};
}

Robot::Robot(Vector2D start, double start_theta, double wheel_r, double track_r, 
          double cartridge_speed_rpm, double gear_r, double m, double i)
    : pos(start), theta(start_theta), wheel_radius(wheel_r), track_radius(track_r),
      cartridge_rpm(cartridge_speed_rpm), gear_ratio(gear_r), mass(m), inertia(i) 
{
    // Initialize state [v_L, v_R]
    X_l = {0.0, 0.0}; // Using X_l as the main state vector for [v_L, v_R]

    double cartridge_rads = cartridge_rpm * (2.0 * M_PI / 60.0);

    double torque_const = STALL_TORQUE / STALL_CURRENT;
    double resistance = VOLTAGE_MAX / STALL_CURRENT;
    double Kv = cartridge_rads / (VOLTAGE_MAX - resistance * FREE_CURRENT);

    D1 = (1.0 / mass + (track_radius * track_radius) / inertia);
    D2 = (1.0 / mass - (track_radius * track_radius) / inertia);

    double G2 = gear_ratio * gear_ratio;
    double r2 = wheel_radius * wheel_radius;
    
    // Assuming simplified model where C1/C2 are same for both sides
    double C1 = -(G2 * torque_const) / (Kv * resistance * r2);
    double C2 = (gear_ratio * torque_const) / (resistance * wheel_radius);

    C1_l = C1; C1_r = C1;
    C2_l = C2; C2_r = C2;
}

void Robot::setVoltages(double left, double right) {
    lV = left;
    rV = right;
    // Clamp voltages
    if (lV > VOLTAGE_MAX) lV = VOLTAGE_MAX;
    if (lV < -VOLTAGE_MAX) lV = -VOLTAGE_MAX;
    if (rV > VOLTAGE_MAX) rV = VOLTAGE_MAX;
    if (rV < -VOLTAGE_MAX) rV = -VOLTAGE_MAX;
}

void Robot::update(double dt) {
    // 1. Construct Continuous Matrices A and B
    algebra::Matrix<double, 2, 2> A, B;
    
    A(0,0) = D1 * C1_l; A(0,1) = D2 * C1_l;
    A(1,0) = D2 * C1_l; A(1,1) = D1 * C1_l; 
    
    B(0,0) = D1 * C2_l; B(0,1) = D2 * C2_l;
    B(1,0) = D2 * C2_l; B(1,1) = D1 * C2_l;

    // 2. Discretize
    auto pair = to_discrete<2>(A, B, dt);
    algebra::Matrix<double, 2, 2> Ad = pair.first;
    algebra::Matrix<double, 2, 2> Bd = pair.second;

    // 3. Update State X[k+1] = Ad * X[k] + Bd * u[k]
    // u = [V_l, V_r]^T
    algebra::Matrix<double, 2, 1> u;
    u(0,0) = lV;
    u(1,0) = rV;

    X_l = (Ad * X_l) + (Bd * u); // X_l holds [v_L, v_R]

    double leftSpeed = X_l(0,0);
    double rightSpeed = X_l(1,0);

    // 4. Calculate Kinematics
    double linVel = (leftSpeed + rightSpeed) / 2.0;
    double omega = (rightSpeed - leftSpeed) / (track_radius * 2.0); // rad/s

    // 5. Update Pose
    double vx = linVel * std::cos(theta);
    double vy = linVel * std::sin(theta);

    Vector2D delta(vx * dt, vy * dt);
    
    vel = Vector2D(vx, vy);
    pos += delta;
    theta += omega * dt;

    // Normalize theta
    while (theta > 2*M_PI) theta -= 2*M_PI;
    while (theta < 0) theta += 2*M_PI;
}

}
