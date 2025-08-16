#pragma once

#include "../../ui/ui.hpp"

namespace OpenWars {
    namespace Game {
        class Scene {
          protected:
            OpenWars::UI::Handler* uiHandler = nullptr;

          public:
            virtual ~Scene() {};

            virtual void render() {};
            void setUIHandler(OpenWars::UI::Handler* handler);
            OpenWars::UI::Handler* getUIHandler();
        };
        class MenuScene : public Scene {
          public:
            MenuScene();
            ~MenuScene();

            void render() override;
        };

        class SceneManager {
            Scene* scenario;

          public:
            Scene& getCurrent();
            void changeTo(Scene& target);
            void transition();
        };
    } // namespace Game
} // namespace OpenWars