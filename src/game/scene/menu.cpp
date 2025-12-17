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
    // Create menu items
    std::vector<UI::CarouselItem> menuItems = {
        {"SINGLEPLAYER",
         []() { IO::Logging::log("%s", "Campaign selected"); },
         true,
         Colors::BLUE_400,
         Colors::BLUE_900,
         Colors::BLUE_600},
        {"BATTLE MAPS",
         []() { IO::Logging::log("%s", "Battle Maps selected"); },
         true,
         Colors::AMBER_400,
         Colors::AMBER_900,
         Colors::AMBER_600},
        {"WAR ROOM",
         []() { IO::Logging::log("%s", "War Room selected"); },
         true,
         Colors::RED_400,
         Colors::RED_900,
         Colors::RED_600},
        {"MULTIPLAYER",
         []() { IO::Logging::log("%s", "Versus selected"); },
         true},
        {"DESIGN ROOM",
         []() { IO::Logging::log("%s", "Design Room selected"); },
         Colors::ORANGE_400,
         Colors::ORANGE_900,
         Colors::ORANGE_600}
    };

    // Create carousel positioned on left side
    auto carousel = std::make_unique<UI::CarouselComponent>(
        menuItems,
        Vector2{100.0f, getWindowHeight() / 2.0f},
        "main_menu_carousel"
    );

    // Customize to match AW2 style
    carousel->setItemHeight(50.0f);
    carousel->setItemSpacing(8.0f);
    carousel->setSelectedOffset(60.0f);
    carousel->setFontSize(20.0f);

    uiHandler->addComponent(std::move(carousel));
}

void OpenWars::Game::MenuScene::onEnter() {
    Scene::onEnter();
    initialized = true;
}

void OpenWars::Game::MenuScene::update(float deltaTime) {
    Scene::update(deltaTime);
}

void OpenWars::Game::MenuScene::render() {
    clearBackground(Colors::ZINC_900);

    // Draw description box at bottom
    float descY = getWindowHeight() - 80;
    float descWidth = getWindowWidth() - 100;
    float descHeight = 60;

    Vector2 descPos = {50, descY + descHeight};

    // Description background
    Utils::Drawing::drawParallelogram(
        descPos,
        descWidth,
        descHeight,
        Theme::SKEW / 4.0f,
        Colors::ZINC_800
    );

    // Description text (placeholder)
    const char* desc = "Fight your way through the story.";
    drawText(desc, 70, (int)(descY + 20), 18, Colors::ZINC_200);

    Scene::render();
}