#include "scene.hpp"
#include "../../io/graphics/graphics.hpp"
#include "../../io/log/logging.hpp"
#include "../../core/drawing/shapes.hpp"

using namespace OpenWars::IO::Graphics;

OpenWars::Game::Scene& OpenWars::Game::SceneManager::getCurrent() {
    return *currentScene;
}

void OpenWars::Game::SceneManager::changeTo(
    OpenWars::Game::Scene& target,
    float transitionDuration
) {
    if(currentScene == &target || transition.active) {
        return;
    }

    nextScene = &target;
    transition.active = true;
    transition.progress = 0.0f;
    transition.duration = transitionDuration;

    // Start exit transition
    if(currentScene) {
        currentScene->onPause();
    }

    IO::Logging::debug("Scene changed to %s.", typeid(target).name());
}

bool OpenWars::Game::SceneManager::handleInput(
    const IO::Input::InputState& state
) {
    if(transition.active) {
        return false;
    }

    if(currentScene && currentScene->getUIHandler()) {
        return currentScene->getUIHandler()->handleInput(state);
    }

    return false;
}

void OpenWars::Game::SceneManager::update(float deltaTime) {
    if(transition.active) {
        updateTransition(deltaTime);
    }

    if(currentScene && !transition.active) {
        currentScene->update(deltaTime);
    }
}

void OpenWars::Game::SceneManager::render() {
    if(currentScene) {
        currentScene->render();
    }

    if(transition.active) {
        renderTransition();
    }
}

void OpenWars::Game::SceneManager::updateTransition(float deltaTime) {
    transition.progress += deltaTime / transition.duration;

    if(transition.progress >= 1.0f) {
        completeTransition();
    }
}

void OpenWars::Game::SceneManager::renderTransition() {
    // Default fade transition
    float alpha = transition.progress < 0.5f
                      ? transition.progress * 2.0f
                      : (1.0f - transition.progress) * 2.0f;

    Drawing::drawRectangle(
        0,
        0,
        getWindowWidth(),
        getWindowHeight(),
        Colors::alpha(Color(0, 0, 0, 255), alpha)
    );
}

void OpenWars::Game::SceneManager::completeTransition() {
    if(currentScene) {
        currentScene->onExit();
    }

    currentScene = nextScene;
    nextScene = nullptr;

    if(currentScene) {
        currentScene->onEnter();
    }

    transition.active = false;

    if(transition.onComplete) {
        transition.onComplete();
    }
}

void OpenWars::Game::FadeTransition::render(float progress) {
    float alpha = progress < 0.5f ? progress * 2.0f : (1.0f - progress) * 2.0f;

    Drawing::drawRectangle(
        0,
        0,
        getWindowWidth(),
        getWindowHeight(),
        Colors::alpha(color, alpha)
    );
}

void OpenWars::Game::SlideTransition::render(float progress) {
    float slideAmount =
        progress < 0.5f ? progress * 2.0f : (1.0f - progress) * 2.0f;

    Drawing::Rectangle rect =
        {0, 0, (float)getWindowWidth(), (float)getWindowHeight()};

    switch(direction) {
    case Left:
        rect.x = -rect.width * (1.0f - slideAmount);
        break;
    case Right:
        rect.x = rect.width * (1.0f - slideAmount);
        break;
    case Up:
        rect.y = -rect.height * (1.0f - slideAmount);
        break;
    case Down:
        rect.y = rect.height * (1.0f - slideAmount);
        break;
    }

    drawRectangleRec(rect, Color(0, 0, 0, 255));
}