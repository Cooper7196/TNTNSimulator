#pragma once

#include <SDL.h>
#include "robot/Robot.hpp"

namespace sim {

class Renderer {
public:
    double scale; // pixels per meter
    int offsetX, offsetY; // offset to center of screen in pixels

    Renderer(int screenWidth, int screenHeight, double fieldWidthMeters) {
        scale = (screenWidth < screenHeight ? screenWidth : screenHeight) / fieldWidthMeters;
        offsetX = screenWidth / 2;
        offsetY = screenHeight / 2;
    }

    void renderField(SDL_Renderer* sdlRenderer) {
        // Draw field boundary
        double fieldSize = 3.6576; // 12ft
        int halfSizePx = (int)(fieldSize / 2.0 * scale);
        
        SDL_Rect fieldRect = { offsetX - halfSizePx, offsetY - halfSizePx, halfSizePx * 2, halfSizePx * 2 };
        SDL_SetRenderDrawColor(sdlRenderer, 100, 100, 100, 255); // Gray boundary
        SDL_RenderDrawRect(sdlRenderer, &fieldRect);

        // Draw grid (2ft x 2ft tiles)
        double tileSize = 0.6096; // 2ft
        SDL_SetRenderDrawColor(sdlRenderer, 50, 50, 50, 255); // Dark gray grid
        for (int i = -3; i <= 3; ++i) {
            int pos = (int)(i * tileSize * scale);
            // Vertical lines
            SDL_RenderDrawLine(sdlRenderer, offsetX + pos, offsetY - halfSizePx, offsetX + pos, offsetY + halfSizePx);
            // Horizontal lines
            SDL_RenderDrawLine(sdlRenderer, offsetX - halfSizePx, offsetY + pos, offsetX + halfSizePx, offsetY + pos);
        }
    }

    void renderRobot(SDL_Renderer* sdlRenderer, const Robot& robot) {
        Vector2D pos = robot.getPos();
        double theta = robot.getTheta();

        // Robot shape: a rectangle
        double w = robot.wheel_radius * 2 * scale; // Placeholder for robot width
        double l = robot.wheel_radius * 4 * scale; // Placeholder for robot length
        // Actually Robot has width/length? No, I removed them in the SS model refactor.
        // I should probably add width/length back to Robot class for rendering.
        
        // For now, let's use a fixed size 0.3m x 0.3m
        double rw = 0.3 * scale;
        double rl = 0.3 * scale;

        // Calculate screen position
        int sx = offsetX + (int)(pos.x * scale);
        int sy = offsetY - (int)(pos.y * scale); // Y is up in physics, down in SDL

        // Draw robot body (rectangle)
        SDL_Rect rect = { (int)(-rw/2), (int)(-rl/2), (int)rw, (int)rl };
        
        // We need to rotate the rectangle. SDL_RenderCopyEx can do this with textures.
        // For simple primitives, we might need to draw lines.
        
        // Let's draw 4 lines for the rectangle
        double c = std::cos(theta);
        double s = std::sin(theta);

        auto rotate = [&](double x, double y) -> SDL_Point {
            return {
                sx + (int)((x * c - y * s) * scale),
                sy - (int)((x * s + y * c) * scale)
            };
        };

        SDL_Point p1 = rotate(-0.15, -0.15);
        SDL_Point p2 = rotate(0.15, -0.15);
        SDL_Point p3 = rotate(0.15, 0.15);
        SDL_Point p4 = rotate(-0.15, 0.15);

        SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255); // White
        SDL_RenderDrawLine(sdlRenderer, p1.x, p1.y, p2.x, p2.y);
        SDL_RenderDrawLine(sdlRenderer, p2.x, p2.y, p3.x, p3.y);
        SDL_RenderDrawLine(sdlRenderer, p3.x, p3.y, p4.x, p4.y);
        SDL_RenderDrawLine(sdlRenderer, p4.x, p4.y, p1.x, p1.y);

        // Draw a line for direction
        SDL_Point front = rotate(0.15, 0);
        SDL_SetRenderDrawColor(sdlRenderer, 255, 0, 0, 255); // Red
        SDL_RenderDrawLine(sdlRenderer, sx, sy, front.x, front.y);
    }

    void clear(SDL_Renderer* sdlRenderer) {
        SDL_SetRenderDrawColor(sdlRenderer, 0, 0, 0, 255); // Black
        SDL_RenderClear(sdlRenderer);
    }

    void present(SDL_Renderer* sdlRenderer) {
        SDL_RenderPresent(sdlRenderer);
    }
};

}
