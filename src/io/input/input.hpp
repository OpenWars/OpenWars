#pragma once

#include <memory>
#include <vector>

namespace raylib {
#include "raylib.h"
}

namespace OpenWars {
    namespace IO {
        namespace Input {
            struct InputState {
                bool W = false;
                bool A = false;
                bool S = false;
                bool D = false;

                raylib::Vector2 mousePos = {0, 0};
                bool pressingLeft = false;
                bool pressingRight = false;
            };

            class BaseHandler {
              public:
                virtual ~BaseHandler() = default;

                virtual void handle(const InputState& state) = 0;
            };

            class PollHandler {
                std::vector<std::unique_ptr<BaseHandler>> boundHandlers;
                InputState state;

              public:
                PollHandler() {
                }

                template <typename T, typename... Args>
                void addHandler(Args&&... args) {
                    boundHandlers.push_back(
                        std::make_unique<T>(std::forward<Args>(args)...)
                    );
                }

                void poll();
                void handle();
            };

        } // namespace Input
    } // namespace IO
} // namespace OpenWars
