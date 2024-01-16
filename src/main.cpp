#include "config/config.cpp"
#include "game/map/map.cpp"
#include "input/mouse_handler.cpp"
#include "raylib.h"
#include "rlgl.h"

void Draw(Camera2D camera, Map::Map parsedMap) {
    ClearBackground(RAYWHITE);

    BeginMode2D(camera);

    Map::draw(parsedMap, GetScreenWidth(), GetScreenHeight());
    EndMode2D();
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "OpenWars");
    Camera2D camera = {{0}};
    camera.zoom = 1.0f;

    SetTargetFPS(60);

    Config::initialize();

    Map::Map parsedMap = Map::parse("map.csv");
    Map::load_images();

    while (!WindowShouldClose()) {
        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
            handle_map_movement(camera);
        handle_zoom(camera);

        BeginDrawing();
        Draw(camera, parsedMap);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
