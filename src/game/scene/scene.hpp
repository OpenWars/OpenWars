#pragma once

namespace OpenWars {
    namespace Game {
        namespace Scenes {
            class Scene{
                virtual void render();
            };
            class MenuScene : Scene {
                void render();
            };
        }
        class SceneManager {
            Scenes::Scene scenario;
            public:
            Scenes::Scene getCurrent();
            void changeTo(Scenes::Scene *target);
            void transition();
        };
    }
}