#pragma once

#include "../ui.hpp"
#include "./button.hpp"
#include "../theme.hpp"

namespace OpenWars::UI {

    class PopupComponent : public Component {
        std::string title;
        std::string message;
        std::vector<std::unique_ptr<ButtonComponent>> buttons;

        struct {
            float width;
            float height;
            Vector2 position;
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
            Color bg = Theme::PRIMARY,
            Color fg = Theme::PRIMARY_FOREGROUND
        );

        void show();
        void close();

        void setTitle(const std::string& newTitle);
        void setMessage(const std::string& newMessage);
    };
} // namespace OpenWars::UI