#pragma once

#include "../ui.hpp"
#include "../theme.hpp"
#include <optional>
#include <string>

namespace raylib {
#include "raylib.h"
}

namespace OpenWars {
    namespace UI {
        class ButtonParent {
          public:
            virtual void handleButtonInput(int id) = 0;
        };

        class ButtonComponent : public Component {
            int id;
            std::string label;
            ButtonParent *parent;
            raylib::Color background;
            raylib::Color foreground;

          public:
            raylib::Vector2 position;
            int width =
                raylib::MeasureText(label.c_str(), 12) + Theme::MARGIN * 2;
            float height = 28;

            ButtonComponent(
                std::string label, raylib::Vector2 position,
                ButtonParent *parent, int id,
                std::optional<raylib::Color> background = Theme::PRIMARY,
                std::optional<raylib::Color> foreground =
                    Theme::PRIMARY_FOREGROUND)
                : id(id), label(label), parent(parent),
                  background(background.value()),
                  foreground(foreground.value()), position(position) {};

            void render() override;
            bool handleInput(const IO::Input::InputState &state) override;
        };

        class PopupComponent : public Component, public ButtonParent {
            std::string title;
            std::string message;
            std::vector<ButtonComponent> buttons;
            bool visible;
            float width = raylib::GetScreenWidth() * 0.666;
            float height = raylib::GetScreenHeight() / 2.f;

          public:
            PopupComponent(const std::string &title, const std::string &msg)
                : title(title), message(msg), visible(true) {};

            void addButton(ButtonComponent btn);
            void setVisible(bool v);
            bool isVisible();
            void render() override;
            void handleButtonInput(int id) override;
            bool handleInput(const IO::Input::InputState &state) override;
        };
    } // namespace UI
} // namespace OpenWars