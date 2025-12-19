#pragma once

#include <string>
#include "../ui.hpp"

namespace OpenWars::UI {

    class ButtonComponent : public Component {
        std::string label;
        Color background;
        Color foreground;
        bool isHovered;
        bool wasHovered;
        float alpha = 1.f;

        struct AnimationState {
            float hoverProgress;
            float clickProgress;
        } animation;

      public:
        ButtonComponent(
            const std::string& label,
            Vector2 position,
            const std::string& id = "",
            Color background = Theme::PRIMARY,
            Color foreground = Theme::PRIMARY_FOREGROUND
        );

        struct ButtonOptions {
            Alignment textAlignment = Alignment::Center;
            float textSize = 14;
        } options;

        void render() override;
        bool handleInput(const IO::Input::InputState& state) override;
        void update(float deltaTime) override;
        void updateLayout() override;
        void setOpacity(float alpha);

        void setLabel(const std::string& newLabel);
        const std::string& getLabel() const {
            return label;
        }

        void onClick(std::function<void()> callback);
    };
}; // namespace OpenWars::UI