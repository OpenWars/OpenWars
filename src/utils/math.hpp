#pragma once

#include "../ui/components/components.hpp"

namespace OpenWars::Utils::Math {
    float calculateButtonAreaHeight(
        std::vector<OpenWars::UI::ButtonComponent*> buttons
    );
} // namespace OpenWars::Utils::Math