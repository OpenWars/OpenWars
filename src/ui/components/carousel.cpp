#include "components.hpp"
#include "../../core/drawing/text.hpp"
#include "../../core/drawing/collision.hpp"
#include "../../core/colors.hpp"
#include "../../utils/drawing.hpp"
#include <algorithm>
#include <cmath>
#include <iterator>

OpenWars::UI::CarouselComponent::CarouselComponent(
    const std::vector<CarouselItem>& items,
    Vector2 position,
    const std::string& id
)
    : Component(id)
    , items(items) {
    layout.x = position.x;
    layout.y = position.y;

    animation.itemOffsets.resize(items.size(), 0.0f);
    animation.targetSelection = 0.0f;
    animation.selectionTransition = 0.0f;

    updateLayout();
}

void OpenWars::UI::CarouselComponent::updateLayout() {
    if(items.empty()) {
        state.bounds = {layout.x, layout.y, 0, 0};
        return;
    }

    float totalHeight = items.size() * options.itemHeight +
                        (items.size() - 1) * options.itemSpacing;

    // Find max width needed (considering selected offset)
    float maxWidth = 0;
    for(const auto& item : items) {
        int textWidth =
            Drawing::measureText(item.label.c_str(), (int)options.fontSize);
        maxWidth = std::max(maxWidth, (float)textWidth);
    }
    maxWidth += options.selectedOffset + Theme::MARGIN * 2;

    state.bounds = {layout.x, layout.y - totalHeight, maxWidth, totalHeight};
}

void OpenWars::UI::CarouselComponent::update(float deltaTime) {
    // Smooth selection transition
    float transitionSpeed = 8.0f;
    animation.selectionTransition +=
        (animation.targetSelection - animation.selectionTransition) *
        deltaTime * transitionSpeed;

    // Update each item's horizontal offset based on whether it's selected
    for(size_t i = 0; i < items.size(); ++i) {
        float targetOffset =
            (i == (size_t)selectedIndex) ? options.selectedOffset : 0.0f;
        animation.itemOffsets[i] +=
            (targetOffset - animation.itemOffsets[i]) * deltaTime * 10.0f;
    }

    // Check if still animating
    state.animating = false;
    for(size_t i = 0; i < items.size(); ++i) {
        float targetOffset =
            (i == (size_t)selectedIndex) ? options.selectedOffset : 0.0f;
        if(std::abs(animation.itemOffsets[i] - targetOffset) > 0.01f) {
            state.animating = true;
            break;
        }
    }
}

void OpenWars::UI::CarouselComponent::render() {
    if(!state.visible || items.empty())
        return;

    int itemCount = items.size();

    int visibleRange =
        std::min(4, itemCount); // increae for more items on viewport

    for(int offset = -visibleRange; offset <= visibleRange; ++offset) {
        int i = (selectedIndex + offset + itemCount) % itemCount;

        const auto& item = items[i];

        bool isSelected = (i == selectedIndex);
        bool isHovered = (i == hoveredIndex);

        int distance = offset;

        float scale = 1.0f;
        float distanceOffset = 0.0f;
        float alphaMultiplier = 1.0f;

        if(!isSelected) {
            // Items further from selection get smaller and shift left
            float absDist = std::abs((float)distance);
            scale = std::max(0.4f, 1.0f - absDist * 0.15f);
            distanceOffset = -absDist * 40.0f; // Move left progressively
            alphaMultiplier = std::max(0.3f, 1.0f - absDist * 0.2f);
        }

        // Use offset-based Y position for circular layout
        float itemY =
            layout.y + offset * (options.itemHeight + options.itemSpacing);

        // Smoothly interpolate the X position
        float baseX = layout.x + animation.itemOffsets[i];
        float itemX = baseX + distanceOffset;

        if(isSelected) {
            int fontSize = (int)(options.fontSize * 1.5f);
            int textWidth = Drawing::measureText(item.label.c_str(), fontSize);

            float bgWidth = std::max(300.0f, textWidth + Theme::MARGIN * 4.0f);
            float bgHeight = 60.0f;

            Vector2 bgPos = {
                itemX - Theme::MARGIN * 2,
                itemY + bgHeight / 2.0f
            };

            // Large shadow
            Vector2 shadowPos = {bgPos.x + 4, bgPos.y + 4};
            Utils::Drawing::drawParallelogram(
                shadowPos,
                bgWidth,
                bgHeight,
                Theme::SKEW / 2.0f,
                Colors::alpha(Colors::BLACK, 0.4f)
            );

            // Main background
            Utils::Drawing::drawParallelogram(
                bgPos,
                bgWidth,
                bgHeight,
                Theme::SKEW / 2.0f,
                item.background
            );

            // Inner border for depth
            Vector2 innerPos = {bgPos.x + 3, bgPos.y - 3};
            Utils::Drawing::drawParallelogramOutline(
                innerPos,
                bgWidth - 6,
                bgHeight - 6,
                Theme::SKEW / 2.0f,
                item.border,
                2.0f
            );

            // Outer border
            Utils::Drawing::drawParallelogramOutline(
                bgPos,
                bgWidth,
                bgHeight,
                Theme::SKEW / 2.0f,
                item.outline,
                3.0f
            );

            // Large text for selected
            Drawing::drawText(
                item.label.c_str(),
                (int)(itemX + Theme::MARGIN),
                (int)(itemY - fontSize / 2.0f) - 6,
                fontSize,
                item.enabled ? Colors::WHITE : Colors::ZINC_400
            );

            // Triangle indicator
            float indicatorX = itemX - (Theme::MARGIN * 2);
            float indicatorY = itemY;
            Vector2 v1 = {indicatorX - 8, indicatorY - 8};
            Vector2 v2 = {indicatorX - 8, indicatorY + 8};
            Vector2 v3 = {indicatorX + 2, indicatorY};
            Drawing::drawTriangle(v1, v2, v3, Colors::ZINC_300);

        } else {
            int baseFontSize = (int)options.fontSize;
            int fontSize = (int)(baseFontSize * scale);

            Color textColor =
                item.enabled ? options.normalColor : options.disabledColor;

            if(isHovered && item.enabled) {
                textColor = Colors::brightness(textColor, 0.3f);
            }

            // Apply alpha based on distance
            textColor = Colors::alpha(textColor, alphaMultiplier);

            Drawing::drawText(
                item.label.c_str(),
                (int)itemX,
                (int)(itemY - fontSize / 2.0f),
                fontSize,
                textColor
            );
        }
    }
}

bool OpenWars::UI::CarouselComponent::handleInput(
    const IO::Input::InputState& inputState
) {
    if(!state.visible || !state.enabled || items.empty())
        return false;

    bool consumed = false;

    bool upPressed = inputState.ArrowUp;
    bool downPressed = inputState.ArrowDown;

    if(upPressed && !wasUpPressed) {
        // Wrap to last item when at first
        int newIndex = (selectedIndex - 1 + items.size()) % items.size();
        selectItem(newIndex);
        consumed = true;
    }

    if(downPressed && !wasDownPressed) {
        // Wrap to first item when at last
        int newIndex = (selectedIndex + 1) % items.size();
        selectItem(newIndex);
        consumed = true;
    }

    int oldHovered = hoveredIndex;
    hoveredIndex = -1;

    for(size_t i = 0; i < items.size(); ++i) {
        Drawing::Rectangle itemBounds = getItemBounds(i);
        if(Drawing::checkCollisionPointRec(inputState.mousePos, itemBounds)) {
            hoveredIndex = i;
            break;
        }
    }

    if(oldHovered != hoveredIndex) {
        invalidate();
    }

    if(inputState.pressingLeft && hoveredIndex >= 0) {
        selectItem(hoveredIndex);

        if(items[hoveredIndex].enabled && items[hoveredIndex].onSelect) {
            items[hoveredIndex].onSelect();
        }

        consumed = true;
    }

    return consumed;
}

void OpenWars::UI::CarouselComponent::selectItem(int index) {
    if(index < 0 || index >= (int)items.size())
        return;

    if(selectedIndex != index) {
        selectedIndex = index;
        animation.targetSelection = (float)index;
        invalidate();

        dispatchEvent(EventType::Change, &index);
    }
}

void OpenWars::UI::CarouselComponent::addItem(const CarouselItem& item) {
    items.push_back(item);
    animation.itemOffsets.push_back(0.0f);
    updateLayout();
    invalidate();
}

void OpenWars::UI::CarouselComponent::removeItem(int index) {
    if(index < 0 || index >= (int)items.size())
        return;

    items.erase(items.begin() + index);
    animation.itemOffsets.erase(animation.itemOffsets.begin() + index);

    if(selectedIndex >= (int)items.size()) {
        selectedIndex = (int)items.size() - 1;
    }

    updateLayout();
    invalidate();
}

void OpenWars::UI::CarouselComponent::setSelectedIndex(int index) {
    selectItem(index);
}

float OpenWars::UI::CarouselComponent::getItemY(int index) const {
    float startY = layout.y - state.bounds.height / 2.0f;
    return startY + index * (options.itemHeight + options.itemSpacing) +
           options.itemHeight / 2.0f;
}

OpenWars::Drawing::Rectangle
OpenWars::UI::CarouselComponent::getItemBounds(int index) const {
    float itemY = getItemY(index);
    float itemX = layout.x + animation.itemOffsets[index];

    int textWidth =
        Drawing::measureText(items[index].label.c_str(), (int)options.fontSize);

    return Drawing::Rectangle{
        itemX - Theme::MARGIN,
        itemY - options.itemHeight / 2.0f,
        (float)textWidth + Theme::MARGIN * 2,
        options.itemHeight
    };
}