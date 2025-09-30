#include "../../utils/drawing.hpp"
#include "components.hpp"
#include <cmath>
#include <raylib.h>
#include <algorithm>

OpenWars::UI::ButtonComponent::ButtonComponent(
    const std::string& label,
    raylib::Vector2 position,
    const std::string& id,
    raylib::Color bg,
    raylib::Color fg
)
    : Component(id)
    , label(label)
    , background(bg)
    , foreground(fg) {
    // calc size based on text
    int fontSize = 14;
    int textWidth = raylib::MeasureText(label.c_str(), fontSize);

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

    raylib::Color bg = raylib::ColorBrightness(background, hoverFactor);
    raylib::Color outline =
        raylib::ColorBrightness(Colors::ZINC_500, hoverFactor);
    raylib::Color shadow =
        raylib::ColorBrightness(Colors::ZINC_600, hoverFactor);

    // apply click offset
    raylib::Vector2 renderPos = {layout.x, layout.y + clickOffset};

    // shadow (less offset when clicked)
    float shadowOffset = 3.0f * (1.0f - animation.clickProgress * 0.5f);
    raylib::Vector2 shadowPos = {
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
    raylib::Color finalBg =
        state.enabled ? bg : raylib::ColorBrightness(bg, -0.3f);
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
    int textWidth = raylib::MeasureText(label.c_str(), fontSize);
    raylib::Vector2 labelPos = {
        centerX - textWidth / 2.0f,
        centerY - fontSize / 2.0f
    };

    raylib::Color textColor =
        state.enabled ? foreground : raylib::ColorBrightness(foreground, -0.4f);
    raylib::DrawText(
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
            raylib::ColorAlpha(
                Colors::GREEN_400,
                0.5f + 0.5f * std::sin(raylib::GetTime() * 3)
            ),
            2.0f
        );
    }
}

bool OpenWars::UI::ButtonComponent::handleInput(
    const IO::Input::InputState& state
) {
    /*if(!enabled)
        return false;*/

    wasHovered = isHovered;
    isHovered = raylib::CheckCollisionPointRec(state.mousePos, getBounds());

    // Dispatch hover events
    if(isHovered != wasHovered) {
        dispatchEvent(isHovered ? EventType::Hover : EventType::Blur);
        invalidate();
    }

    // Handle click
    if(isHovered && state.pressingLeft) {
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
        int textWidth = raylib::MeasureText(label.c_str(), fontSize);
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