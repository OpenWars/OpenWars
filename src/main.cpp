#include <iostream>
void poto(void){
	std::cout << "<S DEBUG>\n";
	std::cout << "ABCDEFGHIJKLMNOPRSTUVWXYZ\n";
	std::cout << "ABCDEFGHIJKLMNOPRSTUVWXYZ\n";
	std::cout << "ABCDEFGHIJKLMNOPRSTUVWXYZ\n";
	std::cout << "ABCDEFGHIJKLMNOPRSTUVWXYZ\n";
	std::cout << "<E DEBUG>\n";
};

#include "config/config.cpp"
#include "game/map/map.cpp"
#include "input/mouse_handler.cpp"
#include "raylib.h"
#include "rlgl.h"

void Draw(Camera2D camera, Map::map_t parsedMap) {
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
	
    int config_code = Config::initialize();
	
	if(config_code < 0) {
		std::cerr <<	"Exception raised while trying to initialize the "
						"configuration API: " <<
						std::to_string(config_code) <<
						".\n";
		return 1;
	}
	
    Map::map_t parsedMap = Map::parse("map.csv");
	
	poto();
	
    Map::load_images();
	
	
	while(!WindowShouldClose()) {
		/*
        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
            handle_map_movement(camera);
        handle_zoom(camera);

        BeginDrawing();
        Draw(camera, parsedMap);
        EndDrawing();
		*/
    }
    CloseWindow();

    return 0;
}
