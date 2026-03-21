#pragma once
#include <functional>

namespace OpenWars::Game {
    enum class MovementType {
        Infantry,
        Mech,
        Tire,
        Tread,
        Air,
        Sea,
        Lander,
        Pipe
    };
} // namespace OpenWars::Game

namespace std {
    template <> struct hash<OpenWars::Game::MovementType> {
        size_t operator()(OpenWars::Game::MovementType mt) const noexcept {
            return hash<int>{}(static_cast<int>(mt));
        }
    };
} // namespace std