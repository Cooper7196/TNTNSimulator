# Specification: Core 2D VexU Robot Simulator

## 1. Overview
This document specifies the requirements for the initial version of the VexU Robot Simulator. The core goal is to create a 2D simulation environment for developing and testing C++ algorithms for VexU robots.

## 2. Functional Requirements

### 2.1. Simulation Core
- The simulator will operate in a 2D environment.
- It will accurately model the kinematics and dynamics of a standard VexU robot. The physics should be high-fidelity for movement, but simplified for collisions.
- The simulation will provide a real-time 2D visual representation of the robot's position and orientation on a VexU field.

### 2.2. Robot and Environment
- The simulator will include a default VexU robot model.
- It will feature a representation of the standard VexU competition field.

### 2.3. C++ API
- A simple and intuitive C++ API will be provided for robot control.
- The API will allow users to control motors (e.g., set velocity).
- The API will allow users to read data from simulated sensors (e.g., encoders, inertial sensors).
- The API will be extensible to support custom sensors, such as a 2D lidar.

### 2.4. User Interaction
- The simulator will allow for user C++ algorithms to be compiled with the main application.
- The simulator will accept input from a game controller for manual control of the robot.

## 3. Non-Functional Requirements

- **Performance:** The simulator must run efficiently on standard developer hardware without requiring excessive CPU or memory resources.
- **Usability:** The C++ API must be well-documented and easy for developers to integrate with their own code.
- **Determinism:** The simulation must be deterministic, ensuring that the same sequence of inputs produces the same outputs, which is crucial for algorithm testing.

## 4. Tech Stack
- **Language:** C++
- **Graphics:** SDL
- **Build System:** CMake
- **Physics:** Custom-built 2D physics engine focused on movement.
