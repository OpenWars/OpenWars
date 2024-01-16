#include <raylib.h>
#include <string>
#include <vector>

namespace Map {
struct Map {
    int width;
    int height;
    std::vector<std::vector<std::string>> tiles;
};
Map parse(std::string name);
void draw(Map map, int screenWidth, int screenHeight);
void load_images();
} // namespace Map  