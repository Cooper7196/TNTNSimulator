#include <SDL.h>
#include <SDL_main.h>
#include <iostream>
#include "physics/PhysicsEngine.hpp"
#include "robot/Robot.hpp"

using namespace sim;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Could not initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "TNTN Simulator",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_SHOWN
    );

    if (window == nullptr) {
        std::cerr << "Could not create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Initialize Physics
    PhysicsEngine physics;
    
    // Robot Parameters
    Vector2D startPos(0.0, 0.0);
    double startTheta = 0.0;
    double wheelRadius = 0.0508; // 2 inches in meters
    double trackRadius = 0.1524; // 6 inches in meters
    double cartridge = 600.0; // RPM
    double gearRatio = 1.0; 
    double mass = 4.53592; // ~10 lbs in kg
    double inertia = 1.0; 

    Robot robot(startPos, startTheta, wheelRadius, trackRadius, cartridge, gearRatio, mass, inertia);
    
    physics.addRobot(&robot);

    // Set some test voltages
    robot.setVoltages(6.0, 6.0); // 6 Volts

    bool quit = false;
    SDL_Event event;
    
    // Simulation loop
    double dt = 0.02; // 20ms

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Update Physics
        physics.update(dt);

        // Debug Output
        // std::cout << "Pos: " << robot.getPos().getX() << ", " << robot.getPos().getY() << std::endl;
        
        SDL_Delay(20); // ~50 FPS
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}