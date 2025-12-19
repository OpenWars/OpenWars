#include "../../core/colors.hpp"
#include "../../core/drawing/text.hpp"
#include "../../io/graphics/graphics.hpp"
#include "../../io/log/logging.hpp"
#include "../../utils/drawing.hpp"
#include "../../ui/components/components.hpp"
#include "scene.hpp"

using namespace OpenWars::IO::Graphics;
using namespace OpenWars::Drawing;
using namespace OpenWars::UI;

OpenWars::Game::MenuScene::MenuScene()
    : Scene("MainMenu") {
    createUI();
}

void OpenWars::Game::MenuScene::createUI() {
    std::vector<UI::CarouselItem> menuItems = {
        {"SINGLEPLAYER",
         "Play campaign's missions or a match against bots.",
         []() { IO::Logging::log("%s", "Campaign selected"); },
         true,
         Colors::BLUE_400,
         Colors::BLUE_900,
         Colors::BLUE_600},
        {"BATTLE MAPS",
         "View, purchase and manage your battle maps.",
         []() { IO::Logging::log("%s", "Battle Maps selected"); },
         true,
         Colors::AMBER_400,
         Colors::AMBER_900,
         Colors::AMBER_600},
        {"WAR ROOM",
         "Play against selected scenarios and score the highest!",
         []() { IO::Logging::log("%s", "War Room selected"); },
         true,
         Colors::RED_400,
         Colors::RED_900,
         Colors::RED_600},
        {"MULTIPLAYER",
         "Play against other players!",
         []() { IO::Logging::log("%s", "Versus selected"); },
         true},
        {"DESIGN ROOM",
         "Design your own maps!",
         []() { IO::Logging::log("%s", "Design Room selected"); },
         true,
         Colors::ORANGE_400,
         Colors::ORANGE_900,
         Colors::ORANGE_600}
    };

    auto carousel = std::make_unique<UI::CarouselComponent>(
        menuItems,
        Vector2{100.0f, getWindowHeight() / 2.0f},
        "main_menu_carousel"
    );

    uiHandler->addComponent(std::move(carousel));

    // we search again for carousel even when we still have
    // access here, is there a better method?
    updateDescription();
}

void OpenWars::Game::MenuScene::onEnter() {
    Scene::onEnter();
    initialized = true;
}

void OpenWars::Game::MenuScene::update(float deltaTime) {
    updateDescription();
    Scene::update(deltaTime);
}

void OpenWars::Game::MenuScene::updateDescription() {
    auto* carousel = dynamic_cast<UI::CarouselComponent*>(
        uiHandler->findComponentById("main_menu_carousel")
    );

    if(carousel)
        menuState.currentDescription = carousel->currentDescription;
}

void OpenWars::Game::MenuScene::render() {
    clearBackground(Colors::ZINC_900);

    // Draw description box at bottom
    float descY = getWindowHeight() - 80;
    float descWidth = getWindowWidth() - 100;
    float descHeight = 60;

    Vector2 descPos = {50, descY + descHeight};

    Utils::Drawing::drawParallelogram(
        descPos,
        descWidth,
        descHeight,
        Theme::SKEW / 4.0f,
        Colors::ZINC_800
    );

    drawText(
        menuState.currentDescription->c_str(),
        70,
        (int)(descY + 20),
        18,
        Colors::ZINC_200
    );

    Scene::render();
}