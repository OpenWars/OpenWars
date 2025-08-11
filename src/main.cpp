#include "game/scene/scene.hpp"
#include "io/graphics/graphics.hpp"
#include "io/input/input.hpp"
#include "io/log/logging.hpp"
#include "ui/ui.hpp"

using namespace OpenWars;

int main() {
    IO::Logging::init();
    IO::Graphics::init(1024, 512);

    Game::SceneManager sceneManager;
    Game::MenuScene scene;
    IO::Input::PollHandler input;
    sceneManager.changeTo(scene);

    input.addHandler<UI::InputHandler>(*sceneManager.getCurrent().getUIHandler());

    while (!IO::Graphics::shouldClose()) {
        input.poll();
        input.handle();
        IO::Graphics::beginFrame();
        sceneManager.getCurrent().render();
        IO::Graphics::swapBuffers();
    }
    IO::Graphics::exit();
}