#pragma once

#include <memory>
#include <vector>

namespace OpenWars {
    namespace UI {
        class Component {
            public:
            virtual ~Component() = default;

            virtual void render() = 0;
            virtual bool handleInput() {}
        };

        class Handler{
            std::vector<std::unique_ptr<Component>> components;

            public:
            virtual ~Handler() = default;

            void renderOverlay();
            bool handleInput();
            void addComponent(std::unique_ptr<Component> c);
        };
    }
}