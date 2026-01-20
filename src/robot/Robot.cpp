#include "robot/Robot.hpp"
#include <cmath>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace sim {

// Constants from sim.py
constexpr double NOMINAL_VOLTAGE = 12.0;
constexpr double STALL_TORQUE_PER_MOTOR = 2.1286320584285905;
constexpr double STALL_CURRENT_PER_MOTOR = 2.5029144189407573;
constexpr double FREE_CURRENT_PER_MOTOR = 0.13;
constexpr double FREE_SPEED_RPM = 200.0;

// Helper for discretization (Euler approximation: Ad = I + A*dt, Bd = B*dt)
template<int N>
std::pair<algebra::Matrix<double, N, N>, algebra::Matrix<double, N, N>> 
to_discrete(const algebra::Matrix<double, N, N>& A, const algebra::Matrix<double, N, N>& B, double dt) {
    algebra::Matrix<double, N, N> Ad;
    for(int r=0; r<N; ++r) {
        for(int c=0; c<N; ++c) {
            Ad(r,c) = A(r,c) * dt;
        }
        Ad(r,r) += 1.0; 
    }
    
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
    X_l = {0.0, 0.0}; 

    int num_motors_per_side = 4; // Updated from 3 to 4 as requested

    double stall_torque = STALL_TORQUE_PER_MOTOR * num_motors_per_side;
    double stall_current = STALL_CURRENT_PER_MOTOR * num_motors_per_side;
    double free_current = FREE_CURRENT_PER_MOTOR * num_motors_per_side;
    double free_speed_rads = cartridge_rpm * (2.0 * M_PI / 60.0);

    double resistance = NOMINAL_VOLTAGE / stall_current;
    double torque_const = stall_torque / stall_current;
    double angular_vel_const = free_speed_rads / (NOMINAL_VOLTAGE - resistance * free_current);

    D1 = (1.0 / mass + (track_radius * track_radius) / inertia);
    D2 = (1.0 / mass - (track_radius * track_radius) / inertia);

    double G2 = gear_ratio * gear_ratio;
    double r2 = wheel_radius * wheel_radius;
    
    double C1 = -(G2 * torque_const) / (angular_vel_const * resistance * r2);
    double C2 = (gear_ratio * torque_const) / (resistance * wheel_radius);

    C1_l = C1; C1_r = C1;
    C2_l = C2; C2_r = C2;
    
    // Default Tuning
    mu_lat = 0.4; // Sideways friction coefficient
    gravity = 9.81;
}

void Robot::setVoltages(double left, double right) {
    lV = left;
    rV = right;
    if (lV > 12.0) lV = 12.0;
    if (lV < -12.0) lV = -12.0;
    if (rV > 12.0) rV = 12.0;
    if (rV < -12.0) rV = -12.0;
}

void Robot::update(double dt) {
    // 1. Construct Continuous Matrices A and B
    algebra::Matrix<double, 2, 2> A, B;
    A(0,0) = D1 * C1_l; A(0,1) = D2 * C1_l;
    A(1,0) = D2 * C1_l; A(1,1) = D1 * C1_l; 
    
    B(0,0) = D1 * C2_l; B(0,1) = D2 * C2_l;
    B(1,0) = D2 * C2_l; B(1,1) = D1 * C2_l;

    // 1.1 Add Viscous Damping to A matrix
    // Derived from dot(v_L) = -b/m * v - k/I * w * r
    double linDamp = viscous_linear / (2.0 * mass);
    double angDamp = viscous_angular / (2.0 * inertia);

    A(0,0) += (angDamp - linDamp);
    A(0,1) += (-angDamp - linDamp);
    A(1,0) += (-angDamp - linDamp);
    A(1,1) += (angDamp - linDamp);

    // 2. Discretize
    auto pair = to_discrete<2>(A, B, dt);
    algebra::Matrix<double, 2, 2> Ad = pair.first;
    algebra::Matrix<double, 2, 2> Bd = pair.second;

    // 3. Update Forward State (Motor Dynamics)
    algebra::Matrix<double, 2, 1> u;
    u(0,0) = lV;
    u(1,0) = rV;

    X_l = (Ad * X_l) + (Bd * u);

    double leftSpeed = X_l(0,0);
    double rightSpeed = X_l(1,0);

    // 4. Kinematics
    double v_fwd_motor = (leftSpeed + rightSpeed) / 2.0;
    double omega = (rightSpeed - leftSpeed) / (track_radius * 2.0);

    // --- LATERAL DYNAMICS & MOMENTUM CONSERVATION ---

    // 1. Momentum Rotation (Coriolis/Centrifugal effect)
    // As the robot rotates by dTheta, its velocity vector in the world frame stays the same
    // but its components in the LOCAL frame rotate by -dTheta.
    double dTheta = omega * dt;
    double cosDT = std::cos(dTheta);
    double sinDT = std::sin(dTheta);

    // Rotate velocity vector: [v_fwd, v_lat] rotated by -dTheta
    // v_fwd_rot = v_fwd * cos(-dT) - v_lat * sin(-dT) = v_fwd * cos(dT) + v_lat * sin(dT)
    // v_lat_rot = v_fwd * sin(-dT) + v_lat * cos(-dT) = -v_fwd * sin(dT) + v_lat * cos(dT)
    double v_fwd_rotated = v_fwd_motor * cosDT + v_lateral * sinDT;
    double v_lat_rotated = -v_fwd_motor * sinDT + v_lateral * cosDT;

    // 2. Apply Coulomb Friction to Lateral State
    double friction_accel = mu_lat * gravity; 
    double max_friction_delta = friction_accel * dt;

    if (std::abs(v_lat_rotated) <= max_friction_delta) {
        v_lateral = 0.0;
    } else {
        if (v_lat_rotated > 0) v_lateral = v_lat_rotated - max_friction_delta;
        else v_lateral = v_lat_rotated + max_friction_delta;
    }

    // 3. Sync State Space with Rotated Forward Velocity
    // This ensures motors "feel" the momentum diversion and don't gain free speed.
    // X_l = [v_L, v_R]
    X_l(0,0) = v_fwd_rotated - omega * track_radius;
    X_l(1,0) = v_fwd_rotated + omega * track_radius;

    // --- GLOBAL POSITION UPDATE ---

    double vx = v_fwd_rotated * std::cos(theta) - v_lateral * std::sin(theta);
    double vy = v_fwd_rotated * std::sin(theta) + v_lateral * std::cos(theta);

    pos.x += vx * dt;
    pos.y += vy * dt;
    theta += dTheta;

    vel = Vector2D(vx, vy);

    while (theta > 2*M_PI) theta -= 2*M_PI;
    while (theta < 0) theta += 2*M_PI;
}

}