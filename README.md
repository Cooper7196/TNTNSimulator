# TNTN Simulator

A high-fidelity 2D VexU Robot Simulator with accurate physics and manual control.

## Features
- State-space drivetrain physics model (from LemLib).
- 2D field and robot visualization using SDL2.
- Manual control via Game Controller or Keyboard (WASD).
- Simple C++ API for algorithm integration.

## Requirements
- C++ Compiler (supporting C++17)
- CMake (3.10+)
- [vcpkg](https://github.com/microsoft/vcpkg) (for dependency management)

## Building

1. **Set up vcpkg toolchain:**
   Ensure you have the path to your `vcpkg.cmake` toolchain file.

2. **Configure and Build:**
   ```bash
   mkdir build
   cd build
   cmake -DCMAKE_TOOLCHAIN_FILE=[path/to/vcpkg]/scripts/buildsystems/vcpkg.cmake ..
   cmake --build .
   ```

## Running

### Simulator
Run the main executable from the `build` directory:
```bash
./Debug/tntn-simulator.exe
```
- **W/S**: Move Forward/Backward
- **A/D**: Rotate Left/Right
- **Game Controller**: Left Stick (Throttle), Right Stick (Turn)

### Tests
Run the physics verification test:
```bash
./Debug/physics_test.exe
```

## API Documentation
See [docs/API.md](./docs/API.md) for details on the C++ API.
