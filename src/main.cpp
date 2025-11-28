#include "core/config/config.hpp"
#include "core/core.hpp"
#include "game/scene/scene.hpp"
#include "io/graphics/graphics.hpp"
#include "io/input/input.hpp"
#include "io/log/logging.hpp"
using namespace OpenWars;

int main() {
    IO::Logging::init();
    IO::Logging::log(
        "Starting OpenWars engine for %s v%s (testing: %hhd)...",
        OpenWars::NAME,
        OpenWars::VERSION,
        OpenWars::IS_TESTING_BUILD
    );

    IO::Logging::log("%s", "Loading configuration...");
    Config::Manager cfg; // FIXME! Other mods will overwrite OW's config
    cfg.init();
    IO::Logging::log("%s", "Finished loading configuration.");

    IO::Logging::log("%s", "Creating window...");
    IO::Graphics::init(cfg.graphics.vsync, cfg.graphics.multisampling);
    IO::Logging::log("%s", "Finished creating window.");

    IO::Logging::log("%s", "Initializing I/O.");
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