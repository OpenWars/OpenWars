#include "components.hpp"
#include "../../utils/drawing.hpp"
#include <raylib.h>

void OpenWars::UI::PopupComponent::setVisible(bool v) {
    visible = v;
}
bool OpenWars::UI::PopupComponent::isVisible() {
    return visible;
}

void OpenWars::UI::PopupComponent::render() {
    if (!visible) return;

    float x = center.x - width/2;
    float y = center.y - height/2;

    raylib::Vector2 tl = {x + slant, y};
    raylib::Vector2 tr = {x + width - slant, y};
    raylib::Vector2 br = {x + width, y + height};
    raylib::Vector2 bl = {x, y + height};

    raylib::Vector2 btl = {tl.x - 4, tl.y - 4};
    raylib::Vector2 btr = {tr.x + 4, tr.y - 4};
    raylib::Vector2 bbr = {br.x + 4, br.y + 4};
    raylib::Vector2 bbl = {bl.x - 4, bl.y + 4};

    Utils::Drawing::drawQuad(tl, tr, bl, br, raylib::GRAY);
    //Utils::Drawing::drawQuad(tl, tr, br, bl, raylib::RAYWHITE);

    float th = 40;
    float slantTitle = slant * (th / height);

    raylib::Vector2 ttl = tl;
    raylib::Vector2 ttr = tr;
    raylib::Vector2 tbr = { tr.x - slantTitle, tr.y + th };
    raylib::Vector2 tbl = { tl.x + slantTitle, tl.y + th };
    Utils::Drawing::drawQuad(ttl, ttr, tbr, tbl, raylib::LIGHTGRAY);

    raylib::DrawText(title.c_str(), ttl.x + 10, ttl.y + 8, 18, raylib::DARKGRAY);

    raylib::DrawText(message.c_str(), tl.x + 20, tl.y + 60, 18, raylib::DARKGRAY);

    float btnWidth = 80;
    float btnHeight = 30;
    float btnSlant = 10;
    float btnX = br.x - btnWidth - 20;
    float btnY = br.y - btnHeight - 10;

    ctl = {btnX + btnSlant, btnY};
    ctr = {btnX + btnWidth - btnSlant, btnY};
    cbr = {btnX + btnWidth, btnY + btnHeight};
    cbl = {btnX, btnY + btnHeight};

    Utils::Drawing::drawQuad(ctl, ctr, cbr, cbl, raylib::LIGHTGRAY);
    raylib::DrawText("CLOSE", ctl.x + 8, ctl.y + 6, 16, raylib::DARKGRAY);
}

bool OpenWars::UI::PopupComponent::handleInput() {
    if (raylib::IsMouseButtonPressed(raylib::MOUSE_LEFT_BUTTON) &&
        Utils::Drawing::pointInQuad(raylib::GetMousePosition(), ctl, ctr, cbr, cbl)) {
        visible = false;
        return true;
    }
    return false;
}