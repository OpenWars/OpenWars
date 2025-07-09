#include "game/scene/scene.hpp"
#include "io/graphics/graphics.hpp"
#include "io/log/logging.hpp"

using namespace OpenWars;

int main(){
    IO::Logging::init();
    IO::Graphics::init(512, 512);

    Game::SceneManager sceneManager;
    Game::MenuScene scene;
    sceneManager.changeTo(&scene);

    while (!IO::Graphics::shouldClose()){
        IO::Graphics::beginFrame();
        sceneManager.getCurrent().render();
        IO::Graphics::swapBuffers();
    }
    IO::Graphics::exit();
}