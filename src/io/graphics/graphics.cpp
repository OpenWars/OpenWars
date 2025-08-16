#include "graphics.hpp"
#include <raylib.h>

raylib::Camera3D camera;
void OpenWars::IO::Graphics::init(int vsync, bool multisampling) {
    raylib::SetExitKey(raylib::KEY_NULL);

    unsigned int flags = 0;
    if(multisampling) {
        flags = raylib::FLAG_MSAA_4X_HINT;
    }

    if(vsync == 2) {
        flags |= raylib::FLAG_VSYNC_HINT;
    }

    raylib::SetConfigFlags(flags);
    raylib::InitWindow(1024, 512, "OpenWars Engine");

    while(!raylib::IsWindowReady()) {};

    camera = {
        raylib::Vector3{0, 0, 0},
        raylib::Vector3{0, 0, 0},
        raylib::Vector3{0, 0, 0},
        0.0f,
        0
    };

    if(vsync == 1) {
        int rRate = raylib::GetMonitorRefreshRate(raylib::GetCurrentMonitor());
        raylib::SetTargetFPS(rRate);
    }
}

void OpenWars::IO::Graphics::displayDebug(bool debug, bool fps) {
    if(fps) {
        raylib::DrawFPS(5, 5);
    }

    if(debug) {
        // TODO
    }
}

void OpenWars::IO::Graphics::exit() {
    raylib::CloseWindow();
}

bool OpenWars::IO::Graphics::shouldClose() {
    return raylib::WindowShouldClose();
}

void OpenWars::IO::Graphics::beginFrame() {
    raylib::BeginDrawing();
    raylib::ClearBackground(raylib::BLACK);
}

void OpenWars::IO::Graphics::beginAttached() {
    raylib::BeginMode3D(camera);
}

void OpenWars::IO::Graphics::endAttached() {
    raylib::EndMode3D();
}

raylib::Camera3D OpenWars::IO::Graphics::getCamera() {
    return camera;
}

void OpenWars::IO::Graphics::swapBuffers() {
    raylib::EndDrawing();
}