#include "../../core/colors.hpp"
#include "../../io/graphics/graphics.hpp"
#include "scene.hpp"

using namespace OpenWars::IO::Graphics;

OpenWars::Game::MenuScene::MenuScene()
    : Scene("MainMenu") {
    createUI();
}

void OpenWars::Game::MenuScene::createUI() {
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

    // todo

    Scene::render();
}