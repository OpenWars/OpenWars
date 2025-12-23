#pragma once

#include <SDL3/SDL.h>
#include <string>

namespace OpenWars::Drawing {
    class SpriteSheet {
      public:
        // Load a spritesheet image from the embedded assets. Returns nullptr on
        // failure.
        static SpriteSheet* loadFromAssets(
            const std::string& path,
            int frameWidth,
            int frameHeight
        );

        ~SpriteSheet();

        // Draw a frame by its index (0..frameCount-1) at x,y (top-left). Scale
        // applied uniformly.
        void drawFrame(int frameIndex, float x, float y, float scale = 1.0f);

        // Draw a frame into the supplied destination rectangle.
        void drawFrameInto(int frameIndex, const SDL_FRect& dstRect);

        int getFrameCount() const;

      private:
        SpriteSheet() = default;

        SDL_Texture* texture = nullptr;
        int textureWidth = 0;
        int textureHeight = 0;
        int frameW = 0;
        int frameH = 0;
        int cols = 0;
        int rows = 0;
    };
} // namespace OpenWars::Drawing
