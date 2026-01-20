cmake_minimum_required(VERSION 3.10)

project(TNTNSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)

# Core Library
add_library(tntn_core 
    src/robot/Robot.cpp
    src/physics/PhysicsEngine.cpp
)
target_include_directories(tntn_core PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Main Simulator Executable
add_executable(tntn-simulator src/main.cpp)
target_link_libraries(tntn-simulator PRIVATE 
    tntn_core 
    SDL2::SDL2main 
    SDL2::SDL2
    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
)

if(WIN32)
    set_target_properties(tntn-simulator PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
endif()

# Physics Test Executable
add_executable(physics_test tests/physics_test.cpp)
target_link_libraries(physics_test PRIVATE tntn_core)
