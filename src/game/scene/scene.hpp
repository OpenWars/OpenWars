#pragma once

#include "../../ui/ui.hpp"
#include "../../io/input/input.hpp"
#include <memory>
#include <functional>
#include <vector>

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
        explicit Scene(const std::string& name)
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
            if(uiHandler)
                uiHandler->update(deltaTime);
        }
        virtual void render() {
            if(uiHandler)
                uiHandler->renderOverlay();
        }
        virtual void handleInput(const IO::Input::InputState&) {
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

    class SceneManager {
        /*
         * Owned scene pool – scenes live here until replaced or the manager
         * is destroyed.
         */
        std::vector<std::unique_ptr<Scene>> ownedScenes;

        Scene* currentScene = nullptr;
        Scene* nextScene = nullptr;

        struct TransitionState {
            bool active = false;
            float progress = 0.0f;
            float duration = 0.5f;
            std::function<void()> onComplete;
        } transition;

      public:
        static SceneManager& get() {
            static SceneManager inst;
            return inst;
        }

        Scene& getCurrent();

        /*
         * Transfer ownership of a heap-allocated scene, then transition to it.
         */
        void changeToOwned(
            std::unique_ptr<Scene> target,
            float transitionDuration = 0.7f
        );

        /*
         * Convenience template: allocate + own + transition in one call.
         */
        template <typename T, typename... Args>
        void changeTo(float transitionDuration = 0.7f, Args&&... args) {
            changeToOwned(
                std::make_unique<T>(std::forward<Args>(args)...),
                transitionDuration
            );
        }

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

      protected:
        SceneManager() = default;
        ~SceneManager() = default;
        SceneManager(const SceneManager&) = delete;
        SceneManager& operator=(const SceneManager&) = delete;
    };

    // Transition effects

    class TransitionEffect {
      public:
        virtual ~TransitionEffect() = default;
        virtual void render(float progress) = 0;
    };

    class FadeTransition : public TransitionEffect {
        Color color;

      public:
        explicit FadeTransition(Color fadeColor = Color(0, 0, 0, 255))
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
        explicit SlideTransition(Direction dir)
            : direction(dir) {
        }
        void render(float progress) override;
    };
} // namespace OpenWars::Game