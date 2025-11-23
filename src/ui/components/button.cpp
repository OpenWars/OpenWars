#include "../../utils/drawing.hpp"
#include "../../core/drawing/text.hpp"
#include "../../core/drawing/collision.hpp"
#include "../../io/graphics/graphics.hpp"
#include "components.hpp"
#include <cmath>
#include <algorithm>

OpenWars::UI::ButtonComponent::ButtonComponent(
    const std::string& label,
    Vector2 position,
    const std::string& id,
    Color bg,
    Color fg
)
    : Component(id)
    , label(label)
    , background(bg)
    , foreground(fg) {
    // calc size based on text
    int fontSize = 14;
    int textWidth = Drawing::measureText(label.c_str(), fontSize);

    layout.width = textWidth + Theme::MARGIN * 2;
    layout.height = 28;
    layout.x = position.x;
    layout.y = position.y;

    state.bounds = {
        layout.x,
        layout.y - layout.height,
        layout.width + Theme::SKEW / 3.f,
        layout.height
    };

    // init animation state
    animation.hoverProgress = 0.0f;
    animation.clickProgress = 0.0f;
}

void OpenWars::UI::ButtonComponent::updateLayout() {
    // calc bounds if layout changed
    float skew = Theme::SKEW / 3.f;
    state.bounds = {
        layout.x,
        layout.y - layout.height,
        layout.width + skew,
        layout.height
    };
}

void OpenWars::UI::ButtonComponent::setOpacity(float alpha) {
    this->alpha = alpha;
}

void OpenWars::UI::ButtonComponent::update(float deltaTime) {
    // hover animation
    float targetHover = state.hovered ? 1.0f : 0.0f;
    animation.hoverProgress +=
        (targetHover - animation.hoverProgress) * deltaTime * 10.0f;

    // click animation
    if(animation.clickProgress > 0.0f) {
        animation.clickProgress =
            std::max(0.0f, animation.clickProgress - deltaTime * 8.0f);
    }

    // update animation state
    state.animating =
        (std::abs(animation.hoverProgress - targetHover) > 0.01f) ||
        (animation.clickProgress > 0.01f);
}

void OpenWars::UI::ButtonComponent::render() {
    if(!state.visible)
        return;

    int skew = Theme::SKEW / 3.f;

    float hoverFactor = animation.hoverProgress * 0.2f;
    float clickOffset = animation.clickProgress * 2.0f;

    Color bg =
        Colors::alpha(Colors::brightness(background, hoverFactor), alpha);
    Color outline =
        Colors::alpha(Colors::brightness(Colors::ZINC_500, hoverFactor), alpha);
    Color shadow =
        Colors::alpha(Colors::brightness(Colors::ZINC_600, hoverFactor), alpha);

    // apply click offset
    Vector2 renderPos = {layout.x, layout.y + clickOffset};

    // shadow (less offset when clicked)
    float shadowOffset = 3.0f * (1.0f - animation.clickProgress * 0.5f);
    Vector2 shadowPos = {
        renderPos.x + shadowOffset,
        renderPos.y + shadowOffset
    };

    // only render shadow if not disabled
    if(state.enabled) {
        Utils::Drawing::drawParallelogram(
            shadowPos,
            layout.width,
            layout.height,
            skew,
            shadow
        );
    }

    // background
    Color finalBg = state.enabled ? bg : Colors::brightness(bg, -0.3f);
    Utils::Drawing::drawParallelogram(
        renderPos,
        layout.width,
        layout.height,
        skew,
        finalBg
    );

    // outline
    Utils::Drawing::drawParallelogramOutline(
        renderPos,
        layout.width,
        layout.height,
        skew,
        outline,
        1.5f
    );

    // text
    float centerX = renderPos.x + (layout.width + skew) / 2.0f;
    float centerY = renderPos.y - layout.height / 2.0f;

    int fontSize = 14;
    int textWidth = Drawing::measureText(label.c_str(), fontSize);
    Vector2 labelPos = {centerX - textWidth / 2.0f, centerY - fontSize / 2.0f};

    Color textColor =
        state.enabled ? foreground : Colors::brightness(foreground, -0.4f);
    Drawing::drawText(
        label.c_str(),
        labelPos.x,
        labelPos.y,
        fontSize,
        textColor
    );

    // focus indicator
    if(state.focused && state.enabled) {
        Utils::Drawing::drawParallelogramOutline(
            renderPos,
            layout.width,
            layout.height,
            skew,
            Colors::alpha(
                Colors::GREEN_400,
                0.5f + 0.5f * std::sin(IO::Graphics::getTime() * 3)
            ),
            2.0f
        );
    }
}

bool OpenWars::UI::ButtonComponent::handleInput(
    const IO::Input::InputState& inputState
) {
    wasHovered = isHovered;
    isHovered = checkCollisionPointRec(inputState.mousePos, getBounds());

    // Dispatch hover events
    if(isHovered != wasHovered) {
        dispatchEvent(isHovered ? EventType::Hover : EventType::Blur);
        state.hovered = isHovered;
        invalidate();
    }

    // Handle click
    if(isHovered && inputState.pressingLeft) {
        // Trigger click animation
        animation.clickProgress = 1.0f;

        // Dispatch click event
        dispatchEvent(EventType::Click);
        invalidate();

        return true; // Consume input
    }

    return false;
}

void OpenWars::UI::ButtonComponent::setLabel(const std::string& newLabel) {
    if(label != newLabel) {
        label = newLabel;

        // Recalculate size
        int fontSize = 14;
        int textWidth = Drawing::measureText(label.c_str(), fontSize);
        layout.width = textWidth + Theme::MARGIN * 2;

        updateLayout();
        invalidate();
    }
}

void OpenWars::UI::ButtonComponent::onClick(std::function<void()> callback) {
    addEventListener(EventType::Click, [callback](const Event& e) {
        callback();
    });
}