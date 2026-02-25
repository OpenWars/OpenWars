#pragma once

#include "../../core/vector.hpp"

namespace OpenWars::IO::Graphics {
    /**
     * Simple 4x4 matrix for view and projection calculations
     */
    struct Matrix4 {
        float m[4][4];

        Matrix4();
        static Matrix4 identity();
        static Matrix4 translate(const Vector3& v);
        static Matrix4 scale(float s);
        static Matrix4
        perspective(float fov, float aspect, float near, float far);
        static Matrix4 orthographic(
            float left,
            float right,
            float bottom,
            float top,
            float near,
            float far
        );
        static Matrix4
        lookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

        Matrix4 operator*(const Matrix4& other) const;
        Vector3 operator*(const Vector3& v) const;
    };

    /**
     * 2D/3D camera system
     */
    class Camera {
      public:
        enum ProjectionType { Orthographic = 0, Perspective = 1 };

        enum BoundaryMode { Unbounded = 0, Clamped = 1 };

        Camera();

        Vector3 getPosition() const;
        void setPosition(const Vector3& pos);

        Vector3 getTarget() const;
        void setTarget(const Vector3& tgt);

        Vector3 getUp() const;
        void setUp(const Vector3& u);

        float getFOV() const;
        void setFOV(float f);

        ProjectionType getProjection() const;
        void setProjection(ProjectionType type);

        int getViewportWidth() const;
        int getViewportHeight() const;

        void pan(float deltaX, float deltaY);
        void panTo(const Vector3& worldPos, float duration = 0.5f);
        void instantPan(const Vector3& worldPos);

        float getZoom() const;
        void setZoom(float z);
        void zoomTo(float targetZoom, float duration = 0.3f);
        void applyZoom(float amount);

        void fitToArea(
            float x,
            float y,
            float width,
            float height,
            float padding = 0.1f
        );

        void setBoundaries(float minX, float maxX, float minY, float maxY);
        void clearBoundaries();
        void setZoomLimits(float minZoom, float maxZoom);

        void update(float deltaTime);
        bool isAnimating() const;
        void stopAnimations();

        Matrix4 getViewMatrix() const;
        Matrix4 getProjectionMatrix() const;
        Matrix4 getViewProjectionMatrix() const;

        Vector2 worldToScreen(const Vector3& worldPos) const;
        Vector3
        screenToWorld(const Vector2& screenPos, float depth = 0.0f) const;
        Vector3 screenToWorldRay(const Vector2& screenPos) const;

        bool isVisible(const Vector3& position, float radius = 0.0f) const;
        bool isInViewport(const Vector2& screenPos) const;

        // Debug info
        struct DebugInfo {
            Vector3 position;
            Vector3 target;
            float zoom;
            bool panAnimating;
            bool zoomAnimating;
        };
        DebugInfo getDebugInfo() const;

        void reset();

      private:
        Vector3 position;
        Vector3 target;
        Vector3 up;
        float fov;
        ProjectionType projection;
        float zoomLevel;

        bool panAnimating = false;
        Vector3 panStart;
        Vector3 panEnd;
        float panDuration = 0.0f;
        float panElapsed = 0.0f;

        bool zoomAnimating = false;
        float zoomStart = 1.0f;
        float zoomEnd = 1.0f;
        float zoomDuration = 0.0f;
        float zoomElapsed = 0.0f;

        bool hasBoundaries = false;
        float minBoundX = 0.0f, maxBoundX = 100.0f;
        float minBoundY = 0.0f, maxBoundY = 100.0f;
        float minZoom = 0.1f, maxZoom = 10.0f;

        mutable Matrix4 viewMatrix;
        mutable Matrix4 projectionMatrix;
        mutable bool viewMatrixDirty = true;
        mutable bool projMatrixDirty = true;

        float easeInOutCubic(float t) const;
        void invalidateMatrices() {
            viewMatrixDirty = true;
            projMatrixDirty = true;
        }
        void clampToBoundaries();
        void clampZoom();
        void updateViewMatrix() const;
        void updateProjectionMatrix() const;
    };
} // namespace OpenWars::IO::Graphics
