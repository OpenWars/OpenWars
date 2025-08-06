#pragma once

#include "../ui.hpp"
#include <string>

namespace raylib {
    #include "raylib.h"
}

namespace OpenWars {
    namespace UI {
        class PopupComponent : public Component {
            std::string title;
            std::string message;
            bool visible;
            float width = raylib::GetScreenWidth() * 0.666;
            float height = raylib::GetScreenHeight() / 2.f;

            public:
            PopupComponent(const std::string& title, const std::string& msg)
                : title(title), message(msg), visible(true) {}

            void setVisible(bool v);
            bool isVisible();
            void render() override;
            bool handleInput() override;
        };
    }
}