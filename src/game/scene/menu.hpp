#pragma once

#include "scene.hpp"

namespace OpenWars::Game {
    class MenuScene : public Scene {
        struct MenuState {
            std::string currentDescription = "";
        } menuState;

      public:
        MenuScene();

        void onEnter() override;
        void update(float deltaTime) override;
        void render() override;

      private:
        void updateDescription();
        void createUI();
    };
} // namespace OpenWars::Game