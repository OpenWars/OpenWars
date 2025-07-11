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
            raylib::Vector2 center = {256, 256};
            float width = 256;
            float height = 256;
            float slant = 10;  // how much the sides incline
            raylib::Vector2 ctl;
            raylib::Vector2 ctr;
            raylib::Vector2 cbr;
            raylib::Vector2 cbl;

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