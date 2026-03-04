#include "scene.hpp"
#include "../../io/graphics/graphics.hpp"
#include "../../io/log/logging.hpp"
#include "../../core/drawing/shapes.hpp"

using namespace OpenWars::IO::Graphics;

OpenWars::Game::Scene& OpenWars::Game::SceneManager::getCurrent() {
    return *currentScene;
}

void OpenWars::Game::SceneManager::changeToOwned(
    std::unique_ptr<Scene> target,
    float transitionDuration
) {
    if(!target || currentScene == target.get() || transition.active)
        return;

    // Take ownership before grabbing the raw pointer.
    Scene* rawNext = target.get();
    ownedScenes.push_back(std::move(target));

    nextScene = rawNext;
    transition.active = true;
    transition.progress = 0.0f;
    transition.duration = transitionDuration;

    if(currentScene)
        currentScene->onPause();

    IO::Logging::debug("Scene changed to %s.", typeid(*nextScene).name());
}

bool OpenWars::Game::SceneManager::handleInput(
    const IO::Input::InputState& state
) {
    if(transition.active)
        return false;

    if(currentScene)
        currentScene->handleInput(state);

    if(currentScene && currentScene->getUIHandler())
        return currentScene->getUIHandler()->handleInput(state);

    return false;
}

void OpenWars::Game::SceneManager::update(float deltaTime) {
    if(transition.active)
        updateTransition(deltaTime);

    if(currentScene && !transition.active)
        currentScene->update(deltaTime);
}

void OpenWars::Game::SceneManager::render() {
    if(currentScene)
        currentScene->render();

    if(transition.active)
        renderTransition();
}

void OpenWars::Game::SceneManager::updateTransition(float deltaTime) {
    transition.progress += deltaTime / transition.duration;

    if(transition.progress >= 0.5f && currentScene != nextScene) {
        if(currentScene)
            currentScene->onExit();
        currentScene = nextScene;
        if(currentScene)
            currentScene->onEnter();
    }

    if(transition.progress >= 1.0f)
        completeTransition();
}

void OpenWars::Game::SceneManager::renderTransition() {
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
    nextScene = nullptr;
    transition.active = false;

    // Prune scenes that are no longer current (free their memory).
    ownedScenes.erase(
        std::remove_if(
            ownedScenes.begin(),
            ownedScenes.end(),
            [this](const std::unique_ptr<Scene>& s) {
                return s.get() != currentScene;
            }
        ),
        ownedScenes.end()
    );

    if(transition.onComplete)
        transition.onComplete();
}

// ---------------------------------------------------------------------------

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
    float slide = progress < 0.5f ? progress * 2.0f : (1.0f - progress) * 2.0f;

    Drawing::Rectangle rect =
        {0, 0, (float)getWindowWidth(), (float)getWindowHeight()};
    switch(direction) {
    case Left:
        rect.x = -rect.width * (1.0f - slide);
        break;
    case Right:
        rect.x = rect.width * (1.0f - slide);
        break;
    case Up:
        rect.y = -rect.height * (1.0f - slide);
        break;
    case Down:
        rect.y = rect.height * (1.0f - slide);
        break;
    }
    drawRectangleRec(rect, Color(0, 0, 0, 255));
}