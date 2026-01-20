# Tech Stack

## Programming Language
The primary programming language for the simulator's core logic, physics engine, and user-provided algorithms will be **C++**. This choice aligns with the project's goal of enabling the development and testing of C++-based localization and movement algorithms.

## 2D Visualization
For 2D visualization, **SDL (Simple DirectMedia Layer)** will be used. SDL was chosen for its lightweight nature, robust 2D graphics capabilities, and excellent cross-platform compatibility (Windows and macOS), ensuring an easy installation and setup process for users.

## Physics Engine
The simulator will feature a **custom-implemented 2D physics engine**. This engine will be specifically designed to provide a high-fidelity and accurate model for the robot's movement. The approach will draw inspiration from existing projects like `https://github.com/lemlib/v5-sim-engine` to ensure realistic robot behavior. Collision detection, while present for basic interactions, will not be the primary focus of complexity.

## Build System
**CMake** will be utilized as the build system for the C++ project. CMake's cross-platform capabilities and widespread adoption in C++ development will streamline the build process for both the simulator's core components and integrated user algorithms.

## Algorithm Integration
User-developed C++ algorithms will be integrated by allowing users to compile their source and header files directly alongside the simulator's codebase. This straightforward approach eliminates the need for dynamic loading or scripting interfaces, simplifying the development workflow for algorithm testing.