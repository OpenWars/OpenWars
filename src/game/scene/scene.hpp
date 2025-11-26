#pragma once

#include "../../ui/ui.hpp"
#include <memory>
#include <functional>

namespace OpenWars::Game {
    enum class SceneEvent { Enter, Exit, Pause, Resume, Update, Render };

    class Scene {
      protected:
        std::unique_ptr<OpenWars::UI::Handler> uiHandler;
        bool initialized = false;
        std::string sceneName;

        struct SceneState {
            bool active = false;
            bool transitioning = false;
            float transitionProgress = 0.0f;
        } state;

      public:
        Scene(const std::string& name)
            : sceneName(name) {
            uiHandler = std::make_unique<OpenWars::UI::Handler>();
        }
        virtual ~Scene() = default;

        virtual void onEnter() {
            state.active = true;
        }
        virtual void onExit() {
            state.active = false;
        }
        virtual void onPause() {
        }
        virtual void onResume() {
        }

        virtual void update(float deltaTime) {
            if(uiHandler) {
                uiHandler->update(deltaTime);
            }
        }

        virtual void render() {
            if(uiHandler) {
                uiHandler->renderOverlay();
            }
        }

        OpenWars::UI::Handler* getUIHandler() {
            return uiHandler.get();
        }

        const std::string& getName() const {
            return sceneName;
        }
        bool isActive() const {
            return state.active;
        }
    };

    class MenuScene : public Scene {
        struct MenuState {
            int selectedOption = 0;
            bool contentDownloaded = false;
        } menuState;

      public:
        MenuScene();

        void onEnter() override;
        void update(float deltaTime) override;
        void render() override;

      private:
        void createUI();
        void showDownloadPrompt();
    };

    class SceneManager {
        Scene* currentScene = nullptr;
        Scene* nextScene = nullptr;

        struct TransitionState {
            bool active = false;
            float progress = 0.0f;
            float duration = 0.5f;
            std::function<void()> onComplete;
        } transition;

      public:
        Scene& getCurrent();
        void changeTo(Scene& target, float transitionDuration = 0.5f);
        bool handleInput(const IO::Input::InputState& state);
        void update(float deltaTime);
        void render();

        bool isTransitioning() const {
            return transition.active;
        }

      private:
        void updateTransition(float deltaTime);
        void renderTransition();
        void completeTransition();
    };

    class TransitionEffect {
      public:
        virtual ~TransitionEffect() = default;
        virtual void render(float progress) = 0;
    };

    class FadeTransition : public TransitionEffect {
        Color color;

      public:
        FadeTransition(Color fadeColor = Color(0, 0, 0, 255))
            : color(fadeColor) {
        }
        void render(float progress) override;
    };

    class SlideTransition : public TransitionEffect {
      public:
        enum Direction { Left, Right, Up, Down };

      private:
        Direction direction;

      public:
        SlideTransition(Direction dir)
            : direction(dir) {
        }
        void render(float progress) override;
    };
} // namespace OpenWars::Game