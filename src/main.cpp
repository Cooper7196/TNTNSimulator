#include <SDL.h>
#include <SDL_main.h>
#include <iostream>
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

    // Initialize Physics Engine
    PhysicsEngine physics;
    
    // Parameters for a VexU Robot
    Vector2D startPos(0.0, 0.0);
    double startTheta = 0.0;
    double wheelRadius = 1.375 * 0.0254; // 2.75 inch diameter
    double trackRadius = 8.0 * 0.0254; // 16 inch track width (radius is 8")
    double cartridge = 600.0; // 600 RPM cartridge
    double gearRatio = 1.0; 
    double mass = 8; // 8 kg
    double inertia = 0.5; // Estimated for 14lb 16" robot

    Robot robot(startPos, startTheta, wheelRadius, trackRadius, cartridge, gearRatio, mass, inertia);
    physics.addRobot(&robot);

    // Initialize Renderer
    Renderer renderer(800, 600, 3.6576); // 12ft field

    // Load controller mappings if file exists
    if (SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt") < 0) {
        // Not an error if file doesn't exist
    }

    // Game Controller
    SDL_GameController* controller = nullptr;
    SDL_Joystick* joystickFallback = nullptr;
    int numJoysticks = SDL_NumJoysticks();
    std::cout << "--- Joystick Detection ---" << std::endl;
    std::cout << "Number of joysticks found: " << numJoysticks << std::endl;

    for (int i = 0; i < numJoysticks; ++i) {
        const char* name = SDL_JoystickNameForIndex(i);
        std::cout << "Joystick " << i << ": " << (name ? name : "Unknown") << std::endl;
        
        if (SDL_IsGameController(i)) {
            std::cout << "  - Recognized as Game Controller." << std::endl;
            if (!controller) {
                controller = SDL_GameControllerOpen(i);
                if (controller) {
                    renderer.controllerName = SDL_GameControllerName(controller);
                    std::cout << "  - Successfully opened as Game Controller." << std::endl;
                }
            }
        } else {
            std::cout << "  - NOT recognized as Game Controller. Trying raw Joystick open..." << std::endl;
            if (!controller && !joystickFallback) {
                joystickFallback = SDL_JoystickOpen(i);
                if (joystickFallback) {
                    renderer.controllerName = std::string("Raw: ") + SDL_JoystickName(joystickFallback);
                    std::cout << "  - Successfully opened as raw Joystick." << std::endl;
                }
            }
        }
    }
    std::cout << "--------------------------" << std::endl;

    bool quit = false;
    SDL_Event event;
    
    // Keyboard State
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

    // Simulation loop
    double dt = 0.01; // 10ms (Match sim.py)

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            } 
            // Simplified hotplug for now, primary focus is initial detection
        }

        // Handle Input (Tank Drive)
        double leftInput = 0.0;
        double rightInput = 0.0;

        // Keyboard mapping: W/S for Left, Up/Down for Right
        if (keyboardState[SDL_SCANCODE_W]) leftInput += 1.0;
        if (keyboardState[SDL_SCANCODE_S]) leftInput -= 1.0;
        if (keyboardState[SDL_SCANCODE_UP]) rightInput += 1.0;
        if (keyboardState[SDL_SCANCODE_DOWN]) rightInput -= 1.0;

        if (controller) {
            // Controller mapping: Left Stick Y and Right Stick Y
            double cLeft = -SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) / 32767.0;
            double cRight = -SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY) / 32767.0;
            
            // Deadzone
            if (std::abs(cLeft) < 0.1) cLeft = 0.0;
            if (std::abs(cRight) < 0.1) cRight = 0.0;

            leftInput += cLeft;
            rightInput += cRight;
        } else if (joystickFallback) {
            // Raw Joystick Fallback mapping: Axis 1 (Left Y) and Axis 3 (Right Y)
            double cLeft = -SDL_JoystickGetAxis(joystickFallback, 1) / 32767.0;
            double cRight = -SDL_JoystickGetAxis(joystickFallback, 3) / 32767.0;

            if (std::abs(cLeft) < 0.1) cLeft = 0.0;
            if (std::abs(cRight) < 0.1) cRight = 0.0;

            leftInput += cLeft;
            rightInput += cRight;
        }
        
        // Clamp inputs to -1.0 to 1.0
        leftInput = std::max(-1.0, std::min(1.0, leftInput));
        rightInput = std::max(-1.0, std::min(1.0, rightInput));

        // Tank Drive: Apply voltages directly
        double leftVolt = leftInput * 12.0;
        double rightVolt = rightInput * 12.0;

        // Normalize if exceeding 12V to preserve steering intent
        double maxMag = std::max(std::abs(leftVolt), std::abs(rightVolt));
        if (maxMag > 12.0) {
            leftVolt = (leftVolt / maxMag) * 12.0;
            rightVolt = (rightVolt / maxMag) * 12.0;
        }

        robot.setVoltages(leftVolt, rightVolt);

        // Update Physics
        physics.update(dt);

        // Render
        renderer.clear(sdlRenderer);
        renderer.renderField(sdlRenderer);
        renderer.renderRobot(sdlRenderer, robot);
        renderer.renderDebugInfo(sdlRenderer, robot);
        renderer.present(sdlRenderer);
        
        SDL_Delay(10); // ~100 FPS to match 10ms dt
    }

    if (controller) SDL_GameControllerClose(controller);
    if (joystickFallback) SDL_JoystickClose(joystickFallback);
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}