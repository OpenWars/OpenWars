#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "../../core/vector.hpp"
#include "../../core/colors.hpp"

namespace OpenWars::IO::Graphics {
    struct Camera3D {
        Vector3 position;
        Vector3 target;
        Vector3 up;
        float fov;
        int projection;
    };

    // Core functions
    void init(int vsync, bool multisampling);
    void displayDebug(bool debug, bool fps);
    void exit();
    bool shouldClose();
    void beginFrame();
    void beginAttached();
    void endAttached();
    Camera3D getCamera();
    void swapBuffers();

    void clearBackground(Color color);

    // Window functions
    int getScreenWidth();
    int getScreenHeight();
    int getMonitorRefreshRate(int monitor);
    int getCurrentMonitor();

    // Time
    float getFrameTime();
    double getTime();

    // Internal access
    SDL_Renderer* getRenderer();
    SDL_Window* getWindow();
} // namespace OpenWars::IO::Graphics