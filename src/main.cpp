#include "core/config/config.hpp"
#include "game/scene/scene.hpp"
#include "io/graphics/graphics.hpp"
#include "io/input/input.hpp"
#include "io/log/logging.hpp"
#include "ui/ui.hpp"

using namespace OpenWars;

int main() {
    IO::Logging::init();
    Config::Manager cfg;
    cfg.load();

    if(!cfg.load()) {
        cfg.graphics.multisampling = false;
        cfg.graphics.vsync = 2;
        cfg.graphics.showFps = true;
        cfg.graphics.displayDebugInfo = true;

        cfg.save();
    }

    IO::Graphics::init(cfg.graphics.vsync, cfg.graphics.multisampling);

    Game::SceneManager sceneManager;
    Game::MenuScene scene;
    IO::Input::PollHandler input;
    sceneManager.changeTo(scene);

    input.addHandler<UI::InputHandler>(
        sceneManager.getCurrent().getUIHandler()
    );

    while(!IO::Graphics::shouldClose()) {
        input.poll();
        input.handle();
        IO::Graphics::beginFrame();
        sceneManager.getCurrent().render();
        IO::Graphics::swapBuffers();
    }
    IO::Graphics::exit();
}