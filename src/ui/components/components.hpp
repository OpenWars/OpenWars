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
            raylib::Rectangle bounds;

            public:
            PopupComponent(const std::string& title, const std::string& msg, raylib::Rectangle rect)
                : title(title), message(msg), visible(true), bounds(rect) {}

            void setVisible(bool v);
            bool isVisible();
            void render() override;
            bool handleInput() override;
        };
    }
}