#include "spritesheet.hpp"
#include "../assets/assets.hpp"
#include "../../io/graphics/graphics.hpp"
#include "../../io/log/logging.hpp"
#include <SDL3_image/SDL_image.h>

using namespace OpenWars::Drawing;

SpriteSheet* SpriteSheet::loadFromAssets(
    const std::string& path,
    int frameWidth,
    int frameHeight
) {
    if(frameWidth <= 0 || frameHeight <= 0) {
        IO::Logging::error(
            false,
            "Invalid frame size for spritesheet: %s",
            path.c_str()
        );
        return nullptr;
    }

    SDL_IOStream* rw = OpenWars::Assets::Manager::get().loadRW(path);
    if(!rw) {
        IO::Logging::error(
            false,
            "Failed to open asset for spritesheet: %s",
            path.c_str()
        );
        return nullptr;
    }

    SDL_Renderer* renderer = IO::Graphics::getRenderer();
    if(!renderer) {
        IO::Logging::error(
            false,
            "Renderer is not initialized when loading spritesheet: %s",
            path.c_str()
        );
        return nullptr;
    }

    SDL_Surface* surface = IMG_Load_IO(rw, 1);
    if(!surface) {
        IO::Logging::error(
            false,
            "IMG_Load_RW failed for %s: %s",
            path.c_str(),
            SDL_GetError()
        );
        return nullptr;
    }

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);
    if(!tex) {
        IO::Logging::error(
            false,
            "SDL_CreateTextureFromSurface failed for %s: %s",
            path.c_str(),
            SDL_GetError()
        );
        SDL_DestroySurface(surface);
        return nullptr;
    }

    // Set nearest-neighbor filtering to prevent texture bleeding between frames
    SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);

    SpriteSheet* ss = new SpriteSheet();
    ss->texture = tex;
    ss->frameW = frameWidth;
    ss->frameH = frameHeight;
    ss->textureWidth = surface->w;
    ss->textureHeight = surface->h;
    SDL_DestroySurface(surface);

    if(ss->textureWidth <= 0 || ss->textureHeight <= 0) {
        IO::Logging::error(
            false,
            "Loaded texture has invalid size: %s",
            path.c_str()
        );
        delete ss;
        return nullptr;
    }

    ss->cols = ss->textureWidth / ss->frameW;
    ss->rows = ss->textureHeight / ss->frameH;

    if(ss->cols <= 0 || ss->rows <= 0) {
        IO::Logging::error(
            false,
            "Frame size doesn't divide texture: %s",
            path.c_str()
        );
        delete ss;
        return nullptr;
    }

    IO::Logging::debug(
        "Loaded spritesheet %s (frames: %dx%d)",
        path.c_str(),
        ss->cols,
        ss->rows
    );
    return ss;
}

SpriteSheet::~SpriteSheet() {
    if(texture)
        SDL_DestroyTexture(texture);
}

int SpriteSheet::getFrameCount() const {
    return cols * rows;
}

void SpriteSheet::setColorMod(uint8_t r, uint8_t g, uint8_t b) {
    if(texture)
        SDL_SetTextureColorMod(texture, r, g, b);
}

void SpriteSheet::resetColorMod() {
    if(texture)
        SDL_SetTextureColorMod(texture, 255, 255, 255);
}

void SpriteSheet::drawFrame(int frameIndex, float x, float y, float scale) {
    if(!texture)
        return;
    if(frameIndex < 0 || frameIndex >= getFrameCount())
        return;

    int col = frameIndex % cols;
    int row = frameIndex / cols;

    // Small inset (0.25px) to prevent bleeding from adjacent frames
    // while minimizing visible border effects
    SDL_FRect src = {
        (float)(col * frameW) + 0.25f,
        (float)(row * frameH) + 0.25f,
        (float)frameW - 0.5f,
        (float)frameH - 0.5f
    };

    SDL_FRect dst = {x, y, frameW * scale, frameH * scale};

    SDL_RenderTexture(IO::Graphics::getRenderer(), texture, &src, &dst);
}

void SpriteSheet::drawFrame(
    int frameIndex,
    float x,
    float y,
    float scale,
    int rotation
) {
    if(rotation == 0) {
        drawFrame(frameIndex, x, y, scale);
        return;
    }

    if(!texture)
        return;
    if(frameIndex < 0 || frameIndex >= getFrameCount())
        return;

    int col = frameIndex % cols;
    int row = frameIndex / cols;

    SDL_FRect src = {
        (float)(col * frameW) + 0.25f,
        (float)(row * frameH) + 0.25f,
        (float)frameW - 0.5f,
        (float)frameH - 0.5f
    };

    float dstW = frameW * scale;
    float dstH = frameH * scale;
    SDL_FRect dst = {x, y, dstW, dstH};

    // Rotate around the tile centre so x,y stays the top-left corner
    // regardless of angle.
    SDL_FPoint centre = {dstW / 2.0f, dstH / 2.0f};
    double angleDeg = 90.0 * rotation;

    SDL_RenderTextureRotated(
        IO::Graphics::getRenderer(),
        texture,
        &src,
        &dst,
        angleDeg,
        &centre,
        SDL_FLIP_NONE
    );
}

void SpriteSheet::drawFrameInto(int frameIndex, const SDL_FRect& dstRect) {
    if(!texture)
        return;
    if(frameIndex < 0 || frameIndex >= getFrameCount())
        return;

    int col = frameIndex % cols;
    int row = frameIndex / cols;

    SDL_FRect src = {
        (float)(col * frameW),
        (float)(row * frameH),
        (float)frameW,
        (float)frameH
    };
    SDL_RenderTexture(IO::Graphics::getRenderer(), texture, &src, &dstRect);
}