#include "../../ui/components/components.hpp"
#include "scene.hpp"
#include <memory>

void OpenWars::Game::Scene::setUIHandler(OpenWars::UI::Handler* handler) {
    uiHandler = handler;
}

OpenWars::UI::Handler* OpenWars::Game::Scene::getUIHandler() {
    return uiHandler;
}

OpenWars::Game::MenuScene::MenuScene() {
    OpenWars::UI::Handler* handler = new OpenWars::UI::Handler();

    auto popup = std::make_unique<UI::PopupComponent>(
        "Download content",
        "It's free! I think... I hope."
    );

    UI::callback_t callback = [](UI::Component* component) {
        static_cast<UI::PopupComponent*>(component)->setVisible(false);
        return;
    };

    popup->addCallback(callback);
    handler->addComponent(std::move(popup));

    setUIHandler(handler);
}

OpenWars::Game::MenuScene::~MenuScene() {
    delete getUIHandler();
}

void OpenWars::Game::MenuScene::render() {
    uiHandler->renderOverlay();
}