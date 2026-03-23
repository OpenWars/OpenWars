#include "core/config/config.hpp"
#include "core/core.hpp"
#include "game/scene/menu.hpp"
#include "game/scene/scene.hpp"
#include "io/graphics/graphics.hpp"
#include "io/input/input.hpp"
#include "io/log/logging.hpp"

using namespace OpenWars;

int main(int argc, char* argv[]) {
    setApp({"OpenWars", "0.0.1", true});

    IO::Logging::init(argc, argv);
    IO::Logging::log(
        "Starting %s v%s (testing: %d)...",
        app().name,
        app().version,
        app().testingBuild
    );

    Config::Manager cfg("openwars");
    cfg.init();

    IO::Graphics::init(cfg.graphics.vsync, cfg.graphics.multisampling);

    IO::Input::Handler input;
    Game::SceneManager::get().changeTo<Game::MenuScene>();

    while(!input.shouldClose()) {
        input.poll();
        IO::Graphics::beginFrame();

        Game::SceneManager::get().handleInput(input.getState());
        Game::SceneManager::get().render();
        Game::SceneManager::get().update(IO::Graphics::getFrameTime());

        IO::Graphics::displayDebug(
            cfg.graphics.displayDebugInfo,
            cfg.graphics.showFps
        );
        IO::Graphics::swapBuffers();
    }

    IO::Graphics::exit();
    return 0;
}