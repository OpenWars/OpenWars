#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "../../core/colors.hpp"
#include "camera.hpp"

namespace OpenWars::IO::Graphics {

    // Core functions
    void init(int vsync, bool multisampling);
    void displayDebug(bool debug, bool fps);
    void exit();
    bool shouldClose();
    void beginFrame();
    void beginAttached();
    void endAttached();
    void swapBuffers();

    void clearBackground(Color color);

    // Camera access
    Camera& getCamera();
    void updateCamera(float deltaTime);

    // Window functions
    int getWindowWidth();
    int getWindowHeight();
    int getMonitorRefreshRate(int monitor);
    int getCurrentMonitor();

    // Time
    float getFrameTime();
    double getTime();

    // Internal access
    SDL_Renderer* getRenderer();
    SDL_Window* getWindow();
} // namespace OpenWars::IO::Graphics