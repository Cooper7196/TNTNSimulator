cmake_minimum_required(VERSION 3.10)

project(TNTNSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

# Core Library
add_library(tntn_core 
    src/robot/Robot.cpp
    src/physics/PhysicsEngine.cpp
)
target_include_directories(tntn_core PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Main Simulator Executable
add_executable(tntn-simulator src/main.cpp)
target_link_libraries(tntn-simulator PRIVATE tntn_core SDL2::SDL2main SDL2::SDL2)

# Physics Test Executable
add_executable(physics_test tests/physics_test.cpp)
target_link_libraries(physics_test PRIVATE tntn_core)
