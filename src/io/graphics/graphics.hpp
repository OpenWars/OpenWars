#pragma once

namespace raylib {
#include "raylib.h"
}

namespace OpenWars::IO::Graphics {
    void init(int vsync, bool multisampling);
    void displayDebug(bool debug, bool fps);
    void exit();
    bool shouldClose();
    void beginFrame();
    /**
        Begins attached drawing in camera.
    */
    void beginAttached();
    /**
        Ends attached drawing.
    */
    void endAttached();
    raylib::Camera3D getCamera();

    void swapBuffers();
} // namespace OpenWars::IO::Graphics