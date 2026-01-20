cmake_minimum_required(VERSION 3.10)

project(TNTNSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)

add_executable(tntn-simulator 
    src/main.cpp
    src/robot/Robot.cpp
    src/physics/PhysicsEngine.cpp
)

target_include_directories(tntn-simulator PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(tntn-simulator PRIVATE SDL2::SDL2main SDL2::SDL2)
