#include "core/config/config.hpp"
#include "game/scene/scene.hpp"
#include "io/graphics/graphics.hpp"
#include "io/input/input.hpp"
#include "io/log/logging.hpp"

using namespace OpenWars;

int main() {
    IO::Logging::init();

    Config::Manager cfg;
    cfg.init();

    IO::Graphics::init(cfg.graphics.vsync, cfg.graphics.multisampling);

    Game::SceneManager sceneManager;
    Game::MenuScene scene;
    IO::Input::Handler input;
    sceneManager.changeTo(scene);

    while(!IO::Graphics::shouldClose()) {
        input.poll();
        IO::Graphics::beginFrame();

        sceneManager.handleInput(input.getState());
        sceneManager.render();
        sceneManager.update(IO::Graphics::getFrameTime());

        IO::Graphics::displayDebug(
            cfg.graphics.displayDebugInfo,
            cfg.graphics.showFps
        );
        IO::Graphics::swapBuffers();
    }
    IO::Graphics::exit();

    return 0;
}