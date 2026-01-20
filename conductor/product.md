# Product Guide

## Initial Concept
I want to build a robot simulator with accurate physics and an easy programmable interface to allow for testing localization and movement algorithms. Said algorithms will be written in C++. We also want to be able to visualize the robot and simulate controlling it.

## Primary Objective
The primary objective of this project is to provide a robust platform for the development and testing of localization and movement algorithms for robots, with a special focus on the Vex robotics ecosystem.

## Target Audience
The simulator is intended for a diverse audience, including:
- Robotics researchers and academics
- Students and educators in robotics courses
- Hobbyists and independent robot developers
- Participants in the VexU Robotics competition

## Key Features
The simulator will provide the following key features:
- **2D Visualization:** A clear and simple 2D visualization of the robot and its environment.
- **C++ Algorithm Integration:** The ability to seamlessly load and execute C++ algorithms for robot control, allowing for rapid testing and iteration.
- **Realistic Development Environment:** The simulator will mimic the real-world robot's programming interface as closely as possible, enabling code to be developed and tested in the simulator and then deployed to a physical robot with minimal changes.

## Physics Simulation
The simulator will feature a 2D physics engine with the following characteristics:
- **High-Fidelity Movement Model:** The core focus of the physics engine will be on providing a highly accurate and realistic model of the robot's movement.
- **Simplified Collision Model:** Complex collision detection and physics are not a primary goal for the initial version.

## Programming Interface
A simple and intuitive C++ API will be provided for interacting with the simulated robot. The API will include:
- **Motor Control:** Functions to control the robot's motors and movement.
- **Sensor Reading:** Functions to read data from a variety of sensors.
- **Custom Sensor Support:** The ability for users to design and integrate their own custom sensors, such as a 2D spinning lidar.