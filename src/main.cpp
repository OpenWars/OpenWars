#include "core/config/config.hpp"
#include "core/core.hpp"
#include "game/scene/menu.hpp"
#include "game/scene/scene.hpp"
#include "io/graphics/graphics.hpp"
#include "io/input/input.hpp"
#include "io/events/events.hpp"
#include "io/log/logging.hpp"
using namespace OpenWars;

int main(int argc, char* argv[]) {
    IO::Logging::init(argc, argv);
    IO::Logging::log(
        "Starting OpenWars engine for %s v%s (testing: %hhd)...",
        OpenWars::NAME,
        OpenWars::VERSION,
        OpenWars::IS_TESTING_BUILD
    );

    IO::Logging::log("%s", "Loading configuration...");
    Config::Manager cfg(OpenWars::CONFIG_DIR);
    cfg.init();
    IO::Logging::log("%s", "Finished loading configuration.");

    IO::Logging::log("%s", "Creating window...");
    IO::Graphics::init(cfg.graphics.vsync, cfg.graphics.multisampling);
    IO::Logging::log("%s", "Finished creating window.");

    IO::Logging::log("%s", "Initializing I/O.");

    IO::Input::Handler input;
    IO::Events::Manager events;
    Game::SceneManager::get().changeTo(*new Game::MenuScene());

    while(!events.shouldClose()) {
        events.poll();
        input.poll(events);
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