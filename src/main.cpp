#include "config/config.cpp"
#include "game/map/map.cpp"
#include "input/mouse_handler.cpp"
#include "raylib.h"
#include "rlgl.h"

void Draw(Camera2D camera) {
    ClearBackground(RAYWHITE);

    BeginMode2D(camera);

    // Draw the 3d grid, rotated 90 degrees and centered around 0,0
    // just so we have something in the XY plane
    rlPushMatrix();
    rlTranslatef(0, 25 * 50, 0);
    rlRotatef(90, 1, 0, 0);
    DrawGrid(100, 50);
    rlPopMatrix();

    // Draw a reference circle
    DrawCircle(100, 100, 50, YELLOW);

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

    while (!WindowShouldClose()) {
        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
            handle_map_movement(camera);
        handle_zoom(camera);

        BeginDrawing();
        Draw(camera);
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
