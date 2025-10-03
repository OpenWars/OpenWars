#pragma once

#include "../ui.hpp"
#include "../theme.hpp"
#include <string>
#include <functional>

namespace OpenWars::UI {
    class ButtonComponent : public Component {
        std::string label;
        IO::Graphics::Color background;
        IO::Graphics::Color foreground;
        bool isHovered;
        bool wasHovered;

        struct AnimationState {
            float hoverProgress;
            float clickProgress;
        } animation;

      public:
        ButtonComponent(
            const std::string& label,
            IO::Graphics::Vector2 position,
            const std::string& id = "",
            IO::Graphics::Color background = Theme::PRIMARY,
            IO::Graphics::Color foreground = Theme::PRIMARY_FOREGROUND
        );

        void render() override;
        bool handleInput(const IO::Input::InputState& state) override;
        void update(float deltaTime) override;
        void updateLayout() override;

        void setLabel(const std::string& newLabel);
        const std::string& getLabel() const {
            return label;
        }

        void onClick(std::function<void()> callback);
    };

    class PopupComponent : public Component {
        std::string title;
        std::string message;
        std::vector<std::unique_ptr<ButtonComponent>> buttons;

        struct {
            float width;
            float height;
            IO::Graphics::Vector2 position;
            bool valid = false;
        } layoutCache;

        struct {
            float showProgress = 0.0f;
            bool closing = false;
        } animation;

      public:
        PopupComponent(
            const std::string& title,
            const std::string& msg,
            const std::string& id = ""
        );

        void render() override;
        bool handleInput(const IO::Input::InputState& state) override;
        void update(float deltaTime) override;
        void updateLayout() override;

        void addButton(
            const std::string& label,
            std::function<void()> callback,
            IO::Graphics::Color bg = Theme::PRIMARY,
            IO::Graphics::Color fg = Theme::PRIMARY_FOREGROUND
        );

        void show();
        void close();

        void setTitle(const std::string& newTitle);
        void setMessage(const std::string& newMessage);
    };

    enum class Alignment { Left, Center, Right, Top, Middle, Bottom };
    struct LayoutOptions {
        float spacing = Theme::MARGIN;
        Alignment horizontalAlign = Alignment::Center;
        Alignment verticalAlign = Alignment::Middle;
        float padding = Theme::MARGIN;
    };

    class LayoutManager {
      public:
        static void layoutHorizontal(
            std::vector<Component*>& components,
            const IO::Graphics::Rectangle& bounds,
            const LayoutOptions& options = LayoutOptions{}
        );

        static void layoutVertical(
            std::vector<Component*>& components,
            const IO::Graphics::Rectangle& bounds,
            const LayoutOptions& options = LayoutOptions{}
        );

        static void layoutGrid(
            std::vector<Component*>& components,
            const IO::Graphics::Rectangle& bounds,
            int columns,
            const LayoutOptions& options = LayoutOptions{}
        );
    };
} // namespace OpenWars::UI