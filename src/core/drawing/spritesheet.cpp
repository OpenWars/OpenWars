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

    // Load surface from RW (IMG will free the RW when freesrc=1)
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

void SpriteSheet::drawFrame(int frameIndex, float x, float y, float scale) {
    if(!texture)
        return;
    if(frameIndex < 0)
        return;

    int maxFrames = getFrameCount();
    if(frameIndex >= maxFrames)
        return;

    int col = frameIndex % cols;
    int row = frameIndex / cols;

    SDL_FRect src = {
        (float)(col * frameW),
        (float)(row * frameH),
        (float)frameW,
        (float)frameH
    };
    SDL_FRect dst = {x, y, frameW * scale, frameH * scale};

    SDL_RenderTexture(IO::Graphics::getRenderer(), texture, &src, &dst);
}

void SpriteSheet::drawFrameInto(int frameIndex, const SDL_FRect& dstRect) {
    if(!texture)
        return;
    if(frameIndex < 0)
        return;

    int maxFrames = getFrameCount();
    if(frameIndex >= maxFrames)
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
