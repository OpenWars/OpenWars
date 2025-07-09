#include "scene.hpp"
#include "../../ui/components/components.hpp"
#include <memory>

namespace raylib {
    #include "raylib.h"
}

void OpenWars::Game::Scene::setUIHandler(OpenWars::UI::Handler* handler) {
    uiHandler = handler;
}

OpenWars::UI::Handler* OpenWars::Game::Scene::getUIHandler() {
    return uiHandler;
}

OpenWars::Game::MenuScene::MenuScene() {
    OpenWars::UI::Handler *handler = new OpenWars::UI::Handler();
    raylib::Rectangle popupRect = { 200, 150, 400, 200 };
    handler->addComponent(std::make_unique<UI::PopupComponent>(
        "Hi", "Test", popupRect
    ));
    
    setUIHandler(handler);
}

OpenWars::Game::MenuScene::~MenuScene() {
    delete getUIHandler();
}

void OpenWars::Game::MenuScene::render() {
    uiHandler->renderOverlay();
}