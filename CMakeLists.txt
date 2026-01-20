cmake_minimum_required(VERSION 3.10)

project(TNTNSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tntn-simulator src/main.cpp)

target_include_directories(tntn-simulator PUBLIC ${PROJECT_SOURCE_DIR}/include)
