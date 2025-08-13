#pragma once

#include "../ui/components/components.hpp"

namespace OpenWars {
    namespace Utils {
        namespace Math {
            float calculateButtonAreaHeight(
                std::vector<OpenWars::UI::ButtonComponent*> buttons);
        }
    } // namespace Utils
} // namespace OpenWars