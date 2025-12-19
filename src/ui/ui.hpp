#pragma once

#include <algorithm>
#include <memory>
#include <vector>
#include <string>
#include <functional>
#include "../io/input/input.hpp"
#include "../core/drawing/shapes.hpp"
#include "theme.hpp"

namespace OpenWars::UI {
    // Forward declarations
    class Handler;
    class Component;

    enum class EventType { Click, Hover, Focus, Blur, Change, Animation };

    struct Event {
        EventType type;
        Component* source;
        void* data;
    };

    using EventCallback = std::function<void(const Event&)>;

    struct ComponentState {
        bool visible = true;
        bool enabled = true;
        bool focused = false;
        bool hovered = false;
        bool animating = false;
        Drawing::Rectangle bounds = {0, 0, 0, 0};
        float opacity = 1.0f;
    };

    class Component {
      protected:
        std::string id;
        ComponentState state;
        Handler* parentHandler = nullptr;
        std::vector<std::pair<EventType, EventCallback>> eventListeners;

        struct Layout {
            float x = 0, y = 0;
            float width = 0, height = 0;
            float minWidth = 0, minHeight = 0;
            float maxWidth = -1, maxHeight = -1;
            float margin = 0;
            float padding = 0;
        } layout;

      public:
        Component(const std::string& id = "")
            : id(id) {
        }
        virtual ~Component() = default;

        virtual void render() = 0;
        virtual bool handleInput(const IO::Input::InputState& state) = 0;
        virtual void update(float deltaTime) {
        }
        virtual void updateLayout() {
        }

        void setVisible(bool visible) {
            if(state.visible != visible) {
                state.visible = visible;
                invalidate();
            }
        }
        bool isVisible() const {
            return state.visible;
        }

        void setEnabled(bool enabled) {
            if(state.enabled != enabled) {
                state.enabled = enabled;
                invalidate();
            }
        }
        bool isEnabled() const {
            return state.enabled;
        }

        bool isAnimating() const {
            return state.animating;
        }

        void addEventListener(EventType type, EventCallback callback) {
            eventListeners.push_back({type, callback});
        }

        void removeEventListener(EventType type) {
            eventListeners.erase(
                std::remove_if(
                    eventListeners.begin(),
                    eventListeners.end(),
                    [type](const auto& pair) { return pair.first == type; }
                ),
                eventListeners.end()
            );
        }

        void setParentHandler(Handler* handler) {
            parentHandler = handler;
        }

        void setBounds(float x, float y, float width, float height) {
            state.bounds = {x, y, width, height};
            layout.x = x;
            layout.y = y;
            layout.width = width;
            layout.height = height;
            invalidate();
        }

        Drawing::Rectangle getBounds() const {
            return state.bounds;
        }

        const std::string& getId() const {
            return id;
        }
        void setId(const std::string& newId) {
            id = newId;
        }

      protected:
        void invalidate();
        void dispatchEvent(EventType type, void* data = nullptr) {
            Event event{type, this, data};
            for(const auto& [listenerType, callback] : eventListeners) {
                if(listenerType == type) {
                    callback(event);
                }
            }
        }
    };

    class Handler {
        std::vector<std::unique_ptr<Component>> components;
        bool needsLayout = false;
        bool needsRedraw = false;

        IO::Input::InputState currentInputState;
        IO::Input::InputState previousInputState;

      public:
        virtual ~Handler() = default;

        void addComponent(std::unique_ptr<Component> c);
        void removeComponent(Component* c);
        Component* findComponentById(const std::string& id);

        void renderOverlay();
        void markDirty();

        bool handleInput(const IO::Input::InputState& state);

        void update(float deltaTime);

        void layoutComponents();

        void bringToFront(Component* component);

      private:
        bool hasAnimatingComponents() const;
    };

    inline void Component::invalidate() {
        if(parentHandler) {
            parentHandler->markDirty();
        }
    }

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
            const Drawing::Rectangle& bounds,
            const LayoutOptions& options = LayoutOptions{}
        );

        static void layoutVertical(
            std::vector<Component*>& components,
            const Drawing::Rectangle& bounds,
            const LayoutOptions& options = LayoutOptions{}
        );

        static void layoutGrid(
            std::vector<Component*>& components,
            const Drawing::Rectangle& bounds,
            int columns,
            const LayoutOptions& options = LayoutOptions{}
        );
    };
} // namespace OpenWars::UI