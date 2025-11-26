#include "ui.hpp"
#include <algorithm>

void OpenWars::UI::Handler::addComponent(std::unique_ptr<Component> c) {
    c->setParentHandler(this);
    components.push_back(std::move(c));
    markDirty();
}

void OpenWars::UI::Handler::removeComponent(Component* c) {
    components.erase(
        std::remove_if(
            components.begin(),
            components.end(),
            [c](const std::unique_ptr<Component>& comp) {
                return comp.get() == c;
            }
        ),
        components.end()
    );
    markDirty();
}

void OpenWars::UI::Handler::markDirty() {
    needsLayout = true;
    needsRedraw = true;
}

void OpenWars::UI::Handler::layoutComponents() {
    if(!needsLayout)
        return;

    // calc layout bounds and component positions
    for(auto& comp : components) {
        comp->updateLayout();
    }

    needsLayout = false;
}

bool OpenWars::UI::Handler::handleInput(const IO::Input::InputState& state) {
    previousInputState = currentInputState;
    currentInputState = state;

    // process in reverse (top -> bottom)
    bool consumed = false;
    for(auto it = components.rbegin(); it != components.rend(); ++it) {
        if((*it)->isVisible() && (*it)->handleInput(state)) {
            consumed = true;
            break; // stop propagation after first handler
        }
    }

    return consumed;
}

void OpenWars::UI::Handler::update(float deltaTime) {
    // anims & transitions
    for(auto& comp : components) {
        if(comp->isVisible()) {
            comp->update(deltaTime);
        }
    }
}

void OpenWars::UI::Handler::renderOverlay() {
    layoutComponents();

    // this was a good idea but doesn't work on raylib
    /* only render if dirty or animated components
    if(!needsRedraw && !hasAnimatingComponents()) {
        return;
    }*/

    // render in draw order
    for(const auto& c : components) {
        if(c->isVisible()) {
            c->render();
        }
    }

    needsRedraw = false;
}

bool OpenWars::UI::Handler::hasAnimatingComponents() const {
    return std::any_of(
        components.begin(),
        components.end(),
        [](const std::unique_ptr<Component>& comp) {
            return comp->isAnimating();
        }
    );
}

OpenWars::UI::Component*
OpenWars::UI::Handler::findComponentById(const std::string& id) {
    auto it = std::find_if(
        components.begin(),
        components.end(),
        [&id](const std::unique_ptr<Component>& comp) {
            return comp->getId() == id;
        }
    );

    return it != components.end() ? it->get() : nullptr;
}

void OpenWars::UI::Handler::bringToFront(Component* component) {
    auto it = std::find_if(
        components.begin(),
        components.end(),
        [component](const std::unique_ptr<Component>& comp) {
            return comp.get() == component;
        }
    );

    if(it != components.end()) {
        auto temp = std::move(*it);
        components.erase(it);
        components.push_back(std::move(temp));
        markDirty();
    }
}