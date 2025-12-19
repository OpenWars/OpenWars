#pragma once

#include <vector>
#include <string>
#include <functional>
#include "../ui.hpp"

namespace OpenWars::UI {

    struct CarouselItem {
        std::string label;
        std::string description;
        std::function<void()> onSelect;
        bool enabled = true;
        Color border = Colors::GREEN_400;
        Color outline = Colors::GREEN_900;
        Color background = Colors::GREEN_700;
    };

    class CarouselComponent : public Component {
        std::vector<CarouselItem> items;
        int selectedIndex = 0;
        int hoveredIndex = -1;

        struct AnimationState {
            float selectionTransition =
                0.0f; // 0-1, smooth transition between items
            float targetSelection = 0.0f;
            std::vector<float> itemOffsets; // Horizontal offset for each item
        } animation;

        struct CarouselOptions {
            float itemHeight = 48.0f;
            float itemSpacing = 12.0f;
            float selectedOffset = 40.0f; // How far right selected items move
            float fontSize = 20.0f;
            float selectedScale = 1.2f;
            Color selectedColor = Theme::PRIMARY_FOREGROUND;
            Color normalColor = Colors::ZINC_400;
            Color disabledColor = Colors::ZINC_600;
        } options;

        bool wasUpPressed = false;
        bool wasDownPressed = false;

      public:
        CarouselComponent(
            const std::vector<CarouselItem>& items,
            Vector2 position,
            const std::string& id = ""
        );

        std::string* currentDescription = &items[0].description; // hack!

        void render() override;
        bool handleInput(const IO::Input::InputState& state) override;
        void update(float deltaTime) override;
        void updateLayout() override;

        void addItem(const CarouselItem& item);
        void removeItem(int index);
        void setSelectedIndex(int index);

        int getSelectedIndex() const {
            return selectedIndex;
        }

        void setItemHeight(float height) {
            options.itemHeight = height;
        }
        void setItemSpacing(float spacing) {
            options.itemSpacing = spacing;
        }
        void setSelectedOffset(float offset) {
            options.selectedOffset = offset;
        }
        void setFontSize(float size) {
            options.fontSize = size;
        }

      private:
        float getItemY(int index) const;
        Drawing::Rectangle getItemBounds(int index) const;
        void selectItem(int index);
    };
} // namespace OpenWars::UI