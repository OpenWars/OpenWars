#include "camera.hpp"
#include "graphics.hpp"
#include <cmath>
#include <algorithm>

namespace OpenWars::IO::Graphics {
    Matrix4::Matrix4() {
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < 4; ++j) {
                m[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
    }

    Matrix4 Matrix4::identity() {
        return Matrix4();
    }

    Matrix4 Matrix4::translate(const Vector3& v) {
        Matrix4 mat;
        mat.m[0][3] = v.x;
        mat.m[1][3] = v.y;
        mat.m[2][3] = v.z;
        return mat;
    }

    Matrix4 Matrix4::scale(float s) {
        Matrix4 mat;
        mat.m[0][0] = s;
        mat.m[1][1] = s;
        mat.m[2][2] = s;
        return mat;
    }

    Matrix4 Matrix4::perspective(
        float fov,
        float aspect,
        float nearPlane,
        float farPlane
    ) {
        Matrix4 mat;
        for(int i = 0; i < 4; ++i)
            for(int j = 0; j < 4; ++j)
                mat.m[i][j] = 0.0f;

        float f = 1.0f / std::tan(fov * 0.5f * 3.14159265f / 180.0f);
        mat.m[0][0] = f / aspect;
        mat.m[1][1] = f;
        mat.m[2][2] = (farPlane + nearPlane) / (nearPlane - farPlane);
        mat.m[2][3] = (2.0f * farPlane * nearPlane) / (nearPlane - farPlane);
        mat.m[3][2] = -1.0f;
        return mat;
    }

    Matrix4 Matrix4::orthographic(
        float left,
        float right,
        float bottom,
        float top,
        float nearPlane,
        float farPlane
    ) {
        Matrix4 mat;
        for(int i = 0; i < 4; ++i)
            for(int j = 0; j < 4; ++j)
                mat.m[i][j] = 0.0f;

        mat.m[0][0] = 2.0f / (right - left);
        mat.m[1][1] = 2.0f / (top - bottom);
        mat.m[2][2] = -2.0f / (farPlane - nearPlane);
        mat.m[0][3] = -(right + left) / (right - left);
        mat.m[1][3] = -(top + bottom) / (top - bottom);
        mat.m[2][3] = -(farPlane + nearPlane) / (farPlane - nearPlane);
        mat.m[3][3] = 1.0f;
        return mat;
    }

    Matrix4 Matrix4::lookAt(
        const Vector3& eye,
        const Vector3& center,
        const Vector3& up
    ) {
        Vector3 f = (center - eye).normalize();
        Vector3 s = f.crossProduct(up).normalize();
        Vector3 u = s.crossProduct(f);

        Matrix4 mat;
        for(int i = 0; i < 4; ++i)
            for(int j = 0; j < 4; ++j)
                mat.m[i][j] = 0.0f;

        mat.m[0][0] = s.x;
        mat.m[0][1] = s.y;
        mat.m[0][2] = s.z;
        mat.m[1][0] = u.x;
        mat.m[1][1] = u.y;
        mat.m[1][2] = u.z;
        mat.m[2][0] = -f.x;
        mat.m[2][1] = -f.y;
        mat.m[2][2] = -f.z;
        mat.m[0][3] = -s.dotProduct(eye);
        mat.m[1][3] = -u.dotProduct(eye);
        mat.m[2][3] = f.dotProduct(eye);
        mat.m[3][3] = 1.0f;
        return mat;
    }

    Matrix4 Matrix4::operator*(const Matrix4& other) const {
        Matrix4 result;
        for(int i = 0; i < 4; ++i) {
            for(int j = 0; j < 4; ++j) {
                result.m[i][j] = 0.0f;
                for(int k = 0; k < 4; ++k) {
                    result.m[i][j] += m[i][k] * other.m[k][j];
                }
            }
        }
        return result;
    }

    Vector3 Matrix4::operator*(const Vector3& v) const {
        Vector3 result;
        result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3];
        result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3];
        result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3];
        return result;
    }

    Camera::Camera()
        : position(0, 0, 10)
        , target(0, 0, 0)
        , up(0, 1, 0)
        , fov(45.0f)
        , projection(Orthographic)
        , zoomLevel(2.0f) // Start at 2x
        , minZoom(1.0f)
        , maxZoom(3.5f) {
    }

    Vector3 Camera::getPosition() const {
        return position;
    }

    void Camera::setPosition(const Vector3& pos) {
        position = pos;
        invalidateMatrices();
    }

    Vector3 Camera::getTarget() const {
        return target;
    }

    void Camera::setTarget(const Vector3& tgt) {
        target = tgt;
        invalidateMatrices();
    }

    Vector3 Camera::getUp() const {
        return up;
    }

    void Camera::setUp(const Vector3& u) {
        up = u;
        invalidateMatrices();
    }

    float Camera::getFOV() const {
        return fov;
    }

    void Camera::setFOV(float f) {
        fov = f;
        invalidateMatrices();
    }

    Camera::ProjectionType Camera::getProjection() const {
        return projection;
    }

    void Camera::setProjection(ProjectionType type) {
        projection = type;
        invalidateMatrices();
    }

    float Camera::getZoom() const {
        return zoomLevel;
    }

    int Camera::getViewportWidth() const {
        return getWindowWidth();
    }

    int Camera::getViewportHeight() const {
        return getWindowHeight();
    }

    void Camera::pan(float deltaX, float deltaY) {
        position.x += deltaX;
        position.y += deltaY;
        target.x += deltaX;
        target.y += deltaY;
        clampToBoundaries();
        invalidateMatrices();
    }

    void Camera::panTo(const Vector3& worldPos, float duration) {
        const float TILE_SIZE = 16.0f;
        Vector3 snappedPos = worldPos;
        snappedPos.x =
            std::floor(worldPos.x / TILE_SIZE) * TILE_SIZE + TILE_SIZE / 2.0f;
        snappedPos.y =
            std::floor(worldPos.y / TILE_SIZE) * TILE_SIZE + TILE_SIZE / 2.0f;

        panStart = position;
        panEnd = snappedPos;
        panDuration = duration;
        panElapsed = 0.0f;
        panAnimating = true;
    }

    void Camera::instantPan(const Vector3& worldPos) {
        const float TILE_SIZE = 16.0f;
        position.x =
            std::floor(worldPos.x / TILE_SIZE) * TILE_SIZE + TILE_SIZE / 2.0f;
        position.y =
            std::floor(worldPos.y / TILE_SIZE) * TILE_SIZE + TILE_SIZE / 2.0f;
        position.z = worldPos.z;

        target = position; // Camera looks at where it is positioned
        clampToBoundaries();
        invalidateMatrices();
    }

    void Camera::setZoom(float z) {
        zoomLevel = z;
        clampZoom();
        invalidateMatrices();
    }

    void Camera::zoomTo(float targetZoom, float duration) {
        zoomStart = zoomLevel;
        zoomEnd = targetZoom;
        zoomDuration = duration;
        zoomElapsed = 0.0f;
        zoomAnimating = true;
    }

    void Camera::applyZoom(float amount) {
        float baseZoom = zoomAnimating ? zoomEnd : zoomLevel;
        float newZoom = std::clamp(baseZoom + amount * 0.3f, minZoom, maxZoom);

        if(std::abs(newZoom - baseZoom) > 0.01f) {
            zoomTo(newZoom, 0.2f);
        }
    }

    void Camera::fitToArea(
        float x,
        float y,
        float width,
        float height,
        float padding
    ) {
        Vector3 center{x + width / 2.0f, y + height / 2.0f, position.z};
        instantPan(center);

        int viewportW = getViewportWidth();
        int viewportH = getViewportHeight();

        float zoomX = (viewportW / width) * (1.0f - padding);
        float zoomY = (viewportH / height) * (1.0f - padding);

        setZoom(std::min(zoomX, zoomY));
    }

    void Camera::setBoundaries(float minX, float maxX, float minY, float maxY) {
        hasBoundaries = true;
        minBoundX = minX;
        maxBoundX = maxX;
        minBoundY = minY;
        maxBoundY = maxY;
        clampToBoundaries();
    }

    void Camera::clearBoundaries() {
        hasBoundaries = false;
    }

    void Camera::setZoomLimits(float minZ, float maxZ) {
        minZoom = minZ;
        maxZoom = maxZ;
        clampZoom();
    }

    void Camera::update(float deltaTime) {
        if(panAnimating) {
            panElapsed += deltaTime;
            float progress = std::min(1.0f, panElapsed / panDuration);

            // todo: does it feel better with easeOutCubic?
            progress = easeOutQuad(progress);

            position = panStart + (panEnd - panStart) * progress;
            target = position; // Keep target at position

            if(panElapsed >= panDuration) {
                panAnimating = false;
                position = panEnd;
                target = position;
            }
            clampToBoundaries();
            invalidateMatrices();
        }

        if(zoomAnimating) {
            zoomElapsed += deltaTime;
            float progress = std::min(1.0f, zoomElapsed / zoomDuration);
            progress = easeOutCubic(progress);
            zoomLevel = zoomStart + (zoomEnd - zoomStart) * progress;

            if(zoomElapsed >= zoomDuration) {
                zoomAnimating = false;
                zoomLevel = zoomEnd;
                clampToBoundaries();
                invalidateMatrices();
            } else {
                invalidateMatrices();
            }
            clampZoom();
        }
    }

    bool Camera::isAnimating() const {
        return panAnimating || zoomAnimating;
    }

    void Camera::stopAnimations() {
        panAnimating = false;
        zoomAnimating = false;
    }

    Matrix4 Camera::getViewMatrix() const {
        if(viewMatrixDirty) {
            updateViewMatrix();
        }
        return viewMatrix;
    }

    Matrix4 Camera::getProjectionMatrix() const {
        if(projMatrixDirty) {
            updateProjectionMatrix();
        }
        return projectionMatrix;
    }

    Matrix4 Camera::getViewProjectionMatrix() const {
        return getProjectionMatrix() * getViewMatrix();
    }

    Vector2 Camera::worldToScreen(const Vector3& worldPos) const {
        Matrix4 viewProj = getViewProjectionMatrix();
        Vector3 screenPos = viewProj * worldPos;

        // Normalize device coordinates to screen space
        int viewportW = getViewportWidth();
        int viewportH = getViewportHeight();

        float screenX = (screenPos.x + 1.0f) * 0.5f * viewportW;
        float screenY = (1.0f - (screenPos.y + 1.0f) * 0.5f) * viewportH;

        return Vector2{screenX, screenY};
    }

    Vector3 Camera::screenToWorld(const Vector2& screenPos, float depth) const {
        // Normalize screen coordinates to [-1, 1]
        int viewportW = getViewportWidth();
        int viewportH = getViewportHeight();

        float normX = (2.0f * screenPos.x) / viewportW - 1.0f;
        float normY = 1.0f - (2.0f * screenPos.y) / viewportH;

        if(projection == Orthographic) {
            float orthoScale = 1.0f / zoomLevel;
            Vector3 worldPos;
            worldPos.x = target.x + normX * (viewportW / 2.0f) * orthoScale;
            worldPos.y = target.y + normY * (viewportH / 2.0f) * orthoScale;
            worldPos.z = depth;
            return worldPos;
        }

        Vector3 worldPos;
        worldPos.x =
            target.x +
            normX * std::tan(fov * 3.14159265f / 180.0f / 2.0f) * depth;
        worldPos.y =
            target.y +
            normY * std::tan(fov * 3.14159265f / 180.0f / 2.0f) * depth /
                ((float)getViewportWidth() / (float)getViewportHeight());
        worldPos.z = depth;
        return worldPos;
    }

    Vector3 Camera::screenToWorldRay(const Vector2& screenPos) const {
        Vector3 nearPoint = screenToWorld(screenPos, 0.0f);
        Vector3 farPoint = screenToWorld(screenPos, 100.0f);
        return (farPoint - nearPoint).normalize();
    }

    bool Camera::isVisible(const Vector3& position, float radius) const {
        Vector2 screenPos = worldToScreen(position);
        int viewportW = getViewportWidth();
        int viewportH = getViewportHeight();

        return screenPos.x + radius > 0 && screenPos.x - radius < viewportW &&
               screenPos.y + radius > 0 && screenPos.y - radius < viewportH;
    }

    bool Camera::isInViewport(const Vector2& screenPos) const {
        int viewportW = getViewportWidth();
        int viewportH = getViewportHeight();
        return screenPos.x >= 0 && screenPos.x < viewportW &&
               screenPos.y >= 0 && screenPos.y < viewportH;
    }

    Camera::DebugInfo Camera::getDebugInfo() const {
        return {position, target, zoomLevel, panAnimating, zoomAnimating};
    }

    void Camera::reset() {
        position = Vector3{0, 0, 10};
        target = Vector3{0, 0, 0};
        up = Vector3{0, 1, 0};
        fov = 45.0f;
        zoomLevel = 2.5f;
        stopAnimations();
        invalidateMatrices();
    }

    float Camera::easeInOutCubic(float t) const {
        if(t < 0.5f) {
            return 4.0f * t * t * t;
        } else {
            float f = 2.0f * t - 2.0f;
            return 0.5f * f * f * f + 1.0f;
        }
    }

    float Camera::easeOutQuad(float t) const {
        return t * (2.0f - t);
    }

    float Camera::easeOutCubic(float t) const {
        float f = 1.0f - t;
        return 1.0f - f * f * f;
    }

    void Camera::clampToBoundaries() {
        if(!hasBoundaries)
            return;

        int viewportW = getViewportWidth();
        int viewportH = getViewportHeight();

        float orthoScale = 1.0f / zoomLevel;
        float halfW = (viewportW / 2.0f) * orthoScale;
        float halfH = (viewportH / 2.0f) * orthoScale;

        float minX = std::min(minBoundX + halfW, maxBoundX - halfW);
        float maxX = std::max(minBoundX + halfW, maxBoundX - halfW);
        float minY = std::min(minBoundY + halfH, maxBoundY - halfH);
        float maxY = std::max(minBoundY + halfH, maxBoundY - halfH);

        position.x = std::clamp(position.x, minX, maxX);
        position.y = std::clamp(position.y, minY, maxY);
    }

    void Camera::clampZoom() {
        zoomLevel = std::clamp(zoomLevel, minZoom, maxZoom);
    }

    void Camera::updateViewMatrix() const {
        viewMatrix = Matrix4::lookAt(position, target, up);
        viewMatrixDirty = false;
    }

    void Camera::updateProjectionMatrix() const {
        int viewportW = getViewportWidth();
        int viewportH = getViewportHeight();
        float aspect = (float)viewportW / (float)viewportH;

        if(projection == Orthographic) {
            float orthoScale = 1.0f / zoomLevel;
            float halfW = (viewportW / 2.0f) * orthoScale;
            float halfH = (viewportH / 2.0f) * orthoScale;
            projectionMatrix = Matrix4::orthographic(
                -halfW,
                halfW,
                -halfH,
                halfH, // ← back to zoomLevel, not zoomEnd
                0.1f,
                1000.0f
            );
        } else {
            projectionMatrix =
                Matrix4::perspective(fov / zoomLevel, aspect, 0.1f, 1000.0f);
        }
        projMatrixDirty = false;
    }

    CameraController::CameraController(Camera* cam)
        : camera(cam) {
    }

    void CameraController::handleInput(const Input::InputState& input) {
        if(!camera)
            return;

        // Handle mouse wheel zoom with Advance Wars style
        if(input.scrollY != 0.0f) {
            // Each scroll step is 0.5 zoom levels
            camera->applyZoom(input.scrollY * 0.5f);
        }

        // Optional: Add keyboard zoom controls (+ and - keys)
        // This would require adding those keys to InputState
    }

    void CameraController::setPanSpeed(float speed) {
        panSpeed = speed;
    }

    void CameraController::setZoomSpeed(float speed) {
        zoomSpeed = speed;
    }

} // namespace OpenWars::IO::Graphics