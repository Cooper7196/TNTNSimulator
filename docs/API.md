# TNTN Simulator C++ API Documentation

## Robot Class

The `Robot` class represents the simulated VexU robot.

### Methods

- `Robot(Vector2D start, double start_theta, double wheel_r, double track_r, double cartridge_rpm, double gear_r, double m, double i)`
  - Constructor to initialize the robot with physical properties and starting pose.
- `void setVoltages(double left, double right)`
  - Sets the motor voltages for the left and right sides of the drivetrain (range -12.0 to 12.0 Volts).
- `void update(double dt)`
  - Updates the robot's physics and pose for a given time step `dt` (in seconds).
- `Vector2D getPos() const`
  - Returns the current position of the robot in meters.
- `double getTheta() const`
  - Returns the current orientation of the robot in radians.
- `Vector2D getVel() const`
  - Returns the current velocity of the robot in meters/second.

## PhysicsEngine Class

The `PhysicsEngine` class manages the simulation of robots.

### Methods

- `void addRobot(Robot* robot)`
  - Adds a robot to the simulation.
- `void update(double dt)`
  - Updates all robots in the simulation for a given time step `dt` (in seconds).

## Vector2D Struct

A simple 2D vector for position and velocity.

### Fields
- `double x`
- `double y`

### Methods
- `double getX() const`
- `double getY() const`
- `double magnitude() const`
- `double theta() const` (Returns angle in radians)
