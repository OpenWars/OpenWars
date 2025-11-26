#include "collision.hpp"

float sign(OpenWars::Vector2 p1, OpenWars::Vector2 p2, OpenWars::Vector2 p3) {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool OpenWars::Drawing::checkCollisionPointRec(
    Vector2 point,
    Drawing::Rectangle rec
) {
    return (
        point.x >= rec.x && point.x <= rec.x + rec.width && point.y >= rec.y &&
        point.y <= rec.y + rec.height
    );
}

bool OpenWars::Drawing::checkCollisionPointTriangle(
    Vector2 p,
    Vector2 v1,
    Vector2 v2,
    Vector2 v3
) {
    float d1 = sign(p, v1, v2);
    float d2 = sign(p, v2, v3);
    float d3 = sign(p, v3, v1);

    bool hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool hasPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(hasNeg && hasPos);
}
