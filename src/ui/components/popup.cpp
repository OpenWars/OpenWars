#include "../../utils/drawing.hpp"
#include "../../core/drawing/collision.hpp"
#include "../../core/drawing/text.hpp"
#include "../../core/colors.hpp"
#include "../../io/graphics/graphics.hpp"
#include "components.hpp"
#include <algorithm>

using namespace OpenWars::IO::Graphics;

OpenWars::UI::PopupComponent::PopupComponent(
    const std::string& title,
    const std::string& msg,
    const std::string& id
)
    : Component(id)
    , title(title)
    , message(msg) {
    // begin hidden
    state.visible = false;

    layoutCache.width = getScreenWidth() * 0.666f;
    layoutCache.height = getScreenHeight() / 2.0f;
}

void OpenWars::UI::PopupComponent::updateLayout() {
    layoutCache.position = {
        (getScreenWidth() - layoutCache.width - Theme::SKEW) / 2.0f,
        (getScreenHeight() + layoutCache.height) / 2.0f
    };

    state.bounds = {
        layoutCache.position.x,
        layoutCache.position.y - layoutCache.height,
        layoutCache.width + Theme::SKEW,
        layoutCache.height
    };

    // buttons
    if(!buttons.empty()) {
        float buttonAreaHeight = 60.0f;
        float buttonY =
            layoutCache.position.y - buttonAreaHeight + Theme::MARGIN;
        float totalButtonWidth = 0;

        for(const auto& btn : buttons) {
            totalButtonWidth += btn->getBounds().width + Theme::MARGIN;
        }
        totalButtonWidth -= Theme::MARGIN;

        float buttonX =
            layoutCache.position.x +
            (layoutCache.width + Theme::SKEW - totalButtonWidth) / 2.0f;

        for(auto& btn : buttons) {
            btn->setBounds(
                buttonX,
                buttonY,
                btn->getBounds().width,
                btn->getBounds().height
            );
            buttonX += btn->getBounds().width + Theme::MARGIN;
        }
    }

    layoutCache.valid = true;
}

void OpenWars::UI::PopupComponent::update(float deltaTime) {
    if(!layoutCache.valid) {
        updateLayout();
    }

    float targetProgress = (state.visible && !animation.closing) ? 1.0f : 0.0f;
    float animSpeed = 8.0f;

    animation.showProgress +=
        (targetProgress - animation.showProgress) * deltaTime * animSpeed;

    state.animating = std::abs(animation.showProgress - targetProgress) > 0.01f;

    if(animation.closing && animation.showProgress < 0.01f) {
        state.visible = false;
        animation.closing = false;
    }

    for(auto& btn : buttons) {
        btn->update(deltaTime);
    }
}

void OpenWars::UI::PopupComponent::render() {
    if(!state.visible || animation.showProgress < 0.01f)
        return;

    Drawing::drawRectangle(
        0,
        0,
        getScreenWidth(),
        getScreenHeight(),
        Colors::alpha(
            Colors::ZINC_950,
            animation.showProgress * animation.showProgress
        )
    );

    float scale = 0.9f + 0.1f * animation.showProgress;
    float yOffset = (1.0f - animation.showProgress) * 50.0f;

    Vector2 pos = {layoutCache.position.x, layoutCache.position.y + yOffset};

    float width = layoutCache.width * scale;
    float height = layoutCache.height * scale;

    pos.x += (layoutCache.width - width) / 2.0f;
    pos.y += (layoutCache.height - height) / 2.0f;

    float shadowAlpha = animation.showProgress;
    Vector2 shadowOffset = {pos.x + 3, pos.y + 3};
    Utils::Drawing::drawParallelogram(
        shadowOffset,
        width,
        height,
        Theme::SKEW,
        Colors::alpha(Colors::ZINC_600, shadowAlpha)
    );

    // Background
    Utils::Drawing::drawParallelogram(
        pos,
        width,
        height,
        Theme::SKEW,
        Colors::ZINC_800
    );

    // Outline
    Utils::Drawing::drawParallelogramOutline(
        pos,
        width,
        height,
        Theme::SKEW,
        Colors::ZINC_600,
        2
    );

    // Title bar
    float titleBarY = pos.y - height + 32;
    float heightFromTop = 32;
    float skewOffsetAtTitleBar = (heightFromTop / height) * Theme::SKEW;
    Vector2 titleBarPos = {
        pos.x + Theme::SKEW - skewOffsetAtTitleBar,
        titleBarY
    };

    Utils::Drawing::drawParallelogram(
        titleBarPos,
        width,
        32,
        skewOffsetAtTitleBar,
        Colors::ZINC_600
    );

    // Title text
    int titleSize = 18;
    int textWidth = Drawing::measureText(title.c_str(), titleSize);
    float centerX = pos.x + (width + Theme::SKEW) / 2.0f;

    Vector2 titlePos = {
        centerX - textWidth / 2.0f,
        pos.y - height + Theme::MARGIN - 6
    };

    Drawing::drawText(
        title.c_str(),
        (int)titlePos.x,
        (int)titlePos.y,
        titleSize,
        Colors::ZINC_200
    );

    // Content with fade - match the background overlay timing
    float contentAlpha = animation.showProgress * animation.showProgress;
    contentAlpha = std::max(0.0f, std::min(1.0f, contentAlpha));

    float topY = pos.y - height + Theme::MARGIN + 32;
    float textAreaWidth = width - (Theme::MARGIN * 2);

    // Temporarily set alpha for text drawing
    Color textColor = Colors::ZINC_100;
    textColor.a = (unsigned char)(255 * contentAlpha);

    // Only draw if visible
    if(contentAlpha > 0.01f) {
        Utils::Drawing::drawTextWrapped(
            message,
            (int)(centerX - textAreaWidth / 2) + Theme::SKEW,
            (int)topY,
            (int)textAreaWidth,
            16,
            textColor
        );
    }

    // Render buttons with stagger animation
    float baseProgress = animation.showProgress * animation.showProgress;

    for(size_t i = 0; i < buttons.size(); ++i) {
        float staggerDelay = i * 0.08f;
        float staggerProgress = std::max(0.0f, baseProgress - staggerDelay);
        float btnAlpha =
            std::min(1.0f, staggerProgress / (1.0f - staggerDelay));

        if(btnAlpha > 0.01f) {
            buttons[i]->setOpacity(btnAlpha);
            buttons[i]->render();
        }
    }
}

bool OpenWars::UI::PopupComponent::handleInput(
    const IO::Input::InputState& inputState
) {
    if(animation.showProgress < 0.5f)
        return false;

    // Check if click is outside popup (for closing)
    bool clickedOutside = false;
    if(inputState.pressingLeft) {
        clickedOutside =
            !Drawing::checkCollisionPointRec(inputState.mousePos, getBounds());
    }

    // Handle button input
    bool consumed = false;
    for(auto& btn : buttons) {
        if(btn->handleInput(inputState)) {
            consumed = true;
            break;
        }
    }

    // Close on outside click if no button handled it
    if(!consumed && clickedOutside) {
        close();
        return true;
    }

    return consumed;
}

void OpenWars::UI::PopupComponent::show() {
    state.visible = true;
    animation.closing = false;
    animation.showProgress = 0.0f;
    layoutCache.valid = false;
    invalidate();

    // Bring to front if we have a parent handler
    if(parentHandler) {
        parentHandler->bringToFront(this);
    }
}

void OpenWars::UI::PopupComponent::close() {
    animation.closing = true;
    invalidate();

    // Dispatch close event
    dispatchEvent(EventType::Change);
}

void OpenWars::UI::PopupComponent::addButton(
    const std::string& label,
    std::function<void()> callback,
    Color bg,
    Color fg
) {
    auto btn = std::make_unique<ButtonComponent>(
        label,
        Vector2{0, 0}, // position set by layout
        id + "_btn_" + std::to_string(buttons.size()),
        bg,
        fg
    );

    btn->onClick([this, callback]() {
        callback();
        close();
    });

    buttons.push_back(std::move(btn));
    layoutCache.valid = false;
    invalidate();
}

void OpenWars::UI::PopupComponent::setTitle(const std::string& newTitle) {
    if(title != newTitle) {
        title = newTitle;
        invalidate();
    }
}

void OpenWars::UI::PopupComponent::setMessage(const std::string& newMessage) {
    if(message != newMessage) {
        message = newMessage;
        invalidate();
    }
}