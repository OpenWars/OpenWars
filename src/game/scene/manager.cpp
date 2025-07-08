#include "scene.hpp"

OpenWars::Game::Scenes::Scene OpenWars::Game::SceneManager::getCurrent(){
    return scenario;
}

void OpenWars::Game::SceneManager::changeTo(OpenWars::Game::Scenes::Scene *target){
    this->transition();
    scenario = *target;
}

void OpenWars::Game::SceneManager::transition(){
    // TODO: Transition screen
}