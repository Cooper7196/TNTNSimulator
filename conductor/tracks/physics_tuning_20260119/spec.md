# Specification: Refine and Tune Physics Engine

## 1. Overview
The current physics implementation results in a robot that moves too fast and drifts significantly (hard to stop turning). This track focuses on analyzing the current state-space model, adding missing physical phenomena like friction, and tuning the parameters for realistic VexU robot behavior.

## 2. Requirements

### 2.1. Physics Analysis and Correction
- Verify the `lemlib` derived state-space model implementation for correctness.
- Ensure the time step `dt` is handled correctly in the discretization.

### 2.2. Friction Implementation
- Implement **Linear Friction**: Opposing force proportional to velocity or constant (Coulomb friction) to slow down the robot when not powered.
- Implement **Angular Friction (Turning Drag)**: Opposing torque to dampen rotation and stop the robot from spinning indefinitely.
- The friction model should be tunable (coefficients).

### 2.3. Parameter Tuning
- Tune the physical constants:
    - Mass (kg)
    - Inertia (kg*m^2)
    - Wheel Radius (m)
    - Track Radius (m)
    - Motor Constants (Stall Torque, Stall Current, etc.)
- Aim for behavior where the robot stops reasonably quickly when input is released (coast mode vs brake mode simulation).

### 2.4. Debug Visualization
- Display real-time data on the screen or console:
    - Linear Velocity (m/s)
    - Angular Velocity (rad/s)
    - Applied Voltages (V)
    - Position (x, y, theta)

## 3. Acceptance Criteria
- Robot acceleration feels realistic (not instant).
- Robot decelerates and stops when input is zero.
- Robot turning is controllable and stops when input is zero (with some inertia).
- Max speed matches expected real-world values for the given gear ratio and motor cartridge.
