#include "scene.hpp"

OpenWars::Game::Scene &OpenWars::Game::SceneManager::getCurrent() {
    return *scenario;
}

void OpenWars::Game::SceneManager::changeTo(OpenWars::Game::Scene &target) {
    this->transition();
    scenario = &target;
}

void OpenWars::Game::SceneManager::transition() {
    // TODO: Transition screen
}