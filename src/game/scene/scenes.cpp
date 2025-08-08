#include "scene.hpp"
#include "../../ui/components/components.hpp"
#include <memory>

void OpenWars::Game::Scene::setUIHandler(OpenWars::UI::Handler* handler) {
    uiHandler = handler;
}

OpenWars::UI::Handler* OpenWars::Game::Scene::getUIHandler() {
    return uiHandler;
}

OpenWars::Game::MenuScene::MenuScene() {
    OpenWars::UI::Handler *handler = new OpenWars::UI::Handler();
    handler->addComponent(std::make_unique<UI::PopupComponent>(
        "Download content", "It's free! I think... I hope."
    ));
    
    setUIHandler(handler);
}

OpenWars::Game::MenuScene::~MenuScene() {
    delete getUIHandler();
}

void OpenWars::Game::MenuScene::render() {
    uiHandler->renderOverlay();
}