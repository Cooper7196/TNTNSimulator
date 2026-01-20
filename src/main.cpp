#include <SDL.h>
#include <SDL_main.h>
#include <iostream>
#include <vector>
#include <map>
#include "physics/PhysicsEngine.hpp"
#include "robot/Robot.hpp"
#include "graphics/Renderer.hpp"

using namespace sim;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
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

    SDL_Renderer* sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (sdlRenderer == nullptr) {
        std::cerr << "Could not create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize Physics
    PhysicsEngine physics;
    
    // Robot Parameters
    Vector2D startPos(0.0, 0.0);
    double startTheta = 0.0;
    double wheelRadius = 0.0508; 
    double trackRadius = 0.1524; 
    double cartridge = 600.0; 
    double gearRatio = 1.0; 
    double mass = 4.53592; 
    double inertia = 1.0; 

    Robot robot(startPos, startTheta, wheelRadius, trackRadius, cartridge, gearRatio, mass, inertia);
    physics.addRobot(&robot);

    // Initialize Renderer
    Renderer renderer(800, 600, 3.6576); // 12ft field

    // Game Controller
    SDL_GameController* controller = nullptr;
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        if (SDL_IsGameController(i)) {
            controller = SDL_GameControllerOpen(i);
            if (controller) {
                std::cout << "Found controller: " << SDL_GameControllerName(controller) << std::endl;
                break;
            }
        }
    }

    bool quit = false;
    SDL_Event event;
    
    // Keyboard State
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

    // Simulation loop
    double dt = 0.02; // 20ms

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_CONTROLLERDEVICEADDED) {
                if (!controller) {
                    controller = SDL_GameControllerOpen(event.cdevice.which);
                }
            } else if (event.type == SDL_CONTROLLERDEVICEREMOVED) {
                if (controller && event.cdevice.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller))) {
                    SDL_GameControllerClose(controller);
                    controller = nullptr;
                }
            }
        }

        // Handle Input
        double throttle = 0.0;
        double turn = 0.0;

        if (controller) {
            // SDL joystick values are -32768 to 32767
            throttle = -SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) / 32767.0;
            turn = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX) / 32767.0;
            
            // Deadzone
            if (std::abs(throttle) < 0.1) throttle = 0.0;
            if (std::abs(turn) < 0.1) turn = 0.0;
        } else {
            // Keyboard fallback (WASD)
            if (keyboardState[SDL_SCANCODE_W]) throttle += 1.0;
            if (keyboardState[SDL_SCANCODE_S]) throttle -= 1.0;
            if (keyboardState[SDL_SCANCODE_A]) turn -= 1.0;
            if (keyboardState[SDL_SCANCODE_D]) turn += 1.0;
        }

        // Arcade Drive: Left = throttle + turn, Right = throttle - turn
        double leftVolt = (throttle + turn) * 12.0;
        double rightVolt = (throttle - turn) * 12.0;
        robot.setVoltages(leftVolt, rightVolt);

        // Update Physics
        physics.update(dt);

        // Render
        renderer.clear(sdlRenderer);
        renderer.renderField(sdlRenderer);
        renderer.renderRobot(sdlRenderer, robot);
        renderer.present(sdlRenderer);
        
        SDL_Delay(20); // ~50 FPS
    }

    if (controller) SDL_GameControllerClose(controller);
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
