#include "../../ui/components/components.hpp"
#include "../../core/colors.hpp"
#include "../../io/graphics/graphics.hpp"
#include "../../core/drawing/text.hpp"
#include "scene.hpp"
#include <memory>

using namespace OpenWars::IO::Graphics;

OpenWars::Game::MenuScene::MenuScene()
    : Scene("MainMenu") {
    createUI();
}

void OpenWars::Game::MenuScene::createUI() {
    auto startBtn = std::make_unique<UI::ButtonComponent>(
        "Start Game",
        Vector2{100, 200},
        "start_button"
    );

    startBtn->onClick([this]() {
        if(!menuState.contentDownloaded) {
            showDownloadPrompt();
        } else {
            // TODO: Start game
        }
    });

    auto optionsBtn = std::make_unique<UI::ButtonComponent>(
        "Options",
        Vector2{100, 250},
        "options_button"
    );

    optionsBtn->onClick([]() {
        // TODO: Open options
    });

    auto exitBtn = std::make_unique<UI::ButtonComponent>(
        "Exit",
        Vector2{100, 300},
        "exit_button",
        UI::Theme::SECONDARY,
        UI::Theme::SECONDARY_FOREGROUND
    );

    exitBtn->onClick([]() {
        // TODO: Exit game
    });

    uiHandler->addComponent(std::move(startBtn));
    uiHandler->addComponent(std::move(optionsBtn));
    uiHandler->addComponent(std::move(exitBtn));
}

void OpenWars::Game::MenuScene::showDownloadPrompt() {
    auto popup = std::make_unique<UI::PopupComponent>(
        "Download Content",
        "Game content needs to be downloaded before playing.\nThis includes "
        "textures, music and game data.\n\nWould you like to download now?",
        "download_popup"
    );

    popup->addButton("Download", [this]() {
        menuState.contentDownloaded = true;
    });

    popup->addButton(
        "Cancel",
        []() {},
        UI::Theme::SECONDARY,
        UI::Theme::SECONDARY_FOREGROUND
    );

    popup->show();
    uiHandler->addComponent(std::move(popup));
}

void OpenWars::Game::MenuScene::onEnter() {
    Scene::onEnter();
    initialized = true;
}

void OpenWars::Game::MenuScene::update(float deltaTime) {
    Scene::update(deltaTime);
    // maybe animations here?
}

void OpenWars::Game::MenuScene::render() {
    clearBackground(Colors::ZINC_900);

    const char* title = "OpenWars";
    int titleSize = 48;
    int titleWidth = Drawing::measureText(title, titleSize);

    Drawing::drawText(
        title,
        getScreenWidth() / 2 - titleWidth / 2,
        50,
        titleSize,
        Colors::GREEN_400
    );

    Scene::render();
}