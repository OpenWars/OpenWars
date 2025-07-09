#pragma once

namespace OpenWars {
    namespace Game {
        class Scene {
            virtual void render();
        };
        class MenuScene : Scene {
            void render();
        };

        class SceneManager {
            Scene scenario;
            public:
            Scene getCurrent();
            void changeTo(Scene *target);
            void transition();
        };
    }
}