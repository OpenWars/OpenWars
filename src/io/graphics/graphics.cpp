#include "graphics.hpp"

raylib::Camera3D camera;
void OpenWars::IO::Graphics::init(int width, int height){
    raylib::SetExitKey(raylib::KEY_NULL);

    raylib::SetConfigFlags(raylib::FLAG_MSAA_4X_HINT);
    raylib::InitWindow(width, height, "OpenWars Engine");

    while (!raylib::IsWindowReady()){};
                
    camera = {
        raylib::Vector3{0,0,0},
        raylib::Vector3{0,0,0},
        raylib::Vector3{0,0,0},
        0.0f, 0
    };

    int r_rate = raylib::GetMonitorRefreshRate(raylib::GetCurrentMonitor());
    raylib::SetTargetFPS(r_rate);
}

void OpenWars::IO::Graphics::exit(){
    raylib::CloseWindow();
}

bool OpenWars::IO::Graphics::shouldClose(){
    return raylib::WindowShouldClose();
}

void OpenWars::IO::Graphics::beginFrame(){
    raylib::BeginDrawing();
    raylib::ClearBackground(raylib::BLACK);
}

void OpenWars::IO::Graphics::beginAttached(){
    raylib::BeginMode3D(camera);
}

void OpenWars::IO::Graphics::endAttached(){
    raylib::EndMode3D();
}

raylib::Camera3D OpenWars::IO::Graphics::getCamera(){
    return camera;
}

void OpenWars::IO::Graphics::swapBuffers(){
    raylib::EndDrawing();
}