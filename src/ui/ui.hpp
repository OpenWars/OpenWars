#pragma once

#include "../io/input/input.hpp"
#include <memory>
#include <vector>

namespace OpenWars::UI {
    class Component {
      public:
        virtual ~Component() = default;

        virtual void render() = 0;
        virtual bool handleInput(const IO::Input::InputState& state) = 0;
    };

    class Handler {
        std::vector<std::unique_ptr<Component>> components;

      public:
        virtual ~Handler() = default;

        void renderOverlay();
        bool handleInput(const IO::Input::InputState& state);
        void addComponent(std::unique_ptr<Component> c);
    };

    class InputHandler : public IO::Input::BaseHandler {
        UI::Handler* ui;

      public:
        InputHandler(UI::Handler* ui)
            : ui(ui) {
        }
        void handle(const IO::Input::InputState& state) override {
            ui->handleInput(state);
        }
    };
} // namespace OpenWars::UI