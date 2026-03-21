#pragma once

#include <SDL3/SDL.h>
#include "../../core/colors.hpp"

namespace OpenWars::IO::Graphics {
    void init(int vsync, bool multisampling);
    void displayDebug(bool debug, bool fps);
    void exit();
    void beginFrame();
    void swapBuffers();
    void clearBackground(Color color);

    int getWindowWidth();
    int getWindowHeight();
    int getMonitorRefreshRate(int monitor);
    int getCurrentMonitor();

    float getFrameTime();
    double getTime();

    SDL_Renderer* getRenderer();
    SDL_Window* getWindow();
} // namespace OpenWars::IO::Graphics