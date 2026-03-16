#include "graphics.hpp"
#include "../../core/colors.hpp"
#include "../../core/core.hpp"
#include "../../core/drawing/text.hpp"
#include "../log/logging.hpp"
#include <SDL3/SDL_init.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <map>

namespace OpenWars::IO::Graphics {
    static SDL_Window* window = nullptr;
    static SDL_Renderer* renderer = nullptr;
    static Camera mainCamera;
    static uint64_t lastFrameTime = 0;
    static float deltaTime = 0.0f;
    static int frameCount = 0;
    static double lastFPSTime = 0.0;
    static int currentFPS = 0;

    // SDL_GetWindowSize() is a non-trivial call (goes through the SDL window
    // subsystem).  We re-query it exactly once per frame in beginFrame() and
    // let every other call in the same frame read the cached values.
    static int cachedWindowWidth = 1024;
    static int cachedWindowHeight = 512;

    void init(int vsync, bool multisampling) {
        SDL_SetAppMetadata(
            OpenWars::NAME,
            OpenWars::VERSION,
            "org.chimoteam.openwars"
        );

        if(!SDL_Init(SDL_INIT_VIDEO)) {
            IO::Logging::error(true, "SDL init failed: %s", SDL_GetError());
            return;
        }

        if(!TTF_Init()) {
            IO::Logging::error(true, "TTF init failed: %s", SDL_GetError());
            return;
        }

        window = SDL_CreateWindow(OpenWars::NAME, 1024, 512, 0);
        if(!window) {
            IO::Logging::error(
                true,
                "Window creation failed: %s",
                SDL_GetError()
            );
            return;
        }

        renderer = SDL_CreateRenderer(window, nullptr);
        if(!renderer) {
            IO::Logging::error(
                true,
                "Renderer creation failed: %s",
                SDL_GetError()
            );
            return;
        }

        SDL_SetRenderVSync(renderer, vsync);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        SDL_GetWindowSize(window, &cachedWindowWidth, &cachedWindowHeight);

        lastFrameTime = SDL_GetTicksNS();
    }

    void displayDebug(bool debug, bool fps) {
        if(fps) {
            Drawing::drawFPS(5, 5, currentFPS);
        }
        if(debug) {
            // TODO: Add debug info
        }
    }

    void exit() {
        Drawing::TextCache::clear();

        for(auto& [size, font] : Drawing::fonts) {
            if(font)
                TTF_CloseFont(font);
        }
        Drawing::fonts.clear();

        if(renderer)
            SDL_DestroyRenderer(renderer);
        if(window)
            SDL_DestroyWindow(window);

        TTF_Quit();
        SDL_Quit();
    }

    bool shouldClose() {
        return false; // driven by event manager
    }

    void beginFrame() {
        uint64_t currentTime = SDL_GetTicksNS();
        deltaTime = (currentTime - lastFrameTime) / 1000000000.0f;
        lastFrameTime = currentTime;

        frameCount++;
        double currentTimeSeconds = currentTime / 1000000000.0;
        if(currentTimeSeconds - lastFPSTime >= 1.0) {
            currentFPS = frameCount;
            frameCount = 0;
            lastFPSTime = currentTimeSeconds;
        }

        if(window)
            SDL_GetWindowSize(window, &cachedWindowWidth, &cachedWindowHeight);

        Drawing::TextCache::tick();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    void beginAttached() {
    }
    void endAttached() {
    }

    void swapBuffers() {
        SDL_RenderPresent(renderer);
    }

    void clearBackground(Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderClear(renderer);
    }

    int getWindowWidth() {
        return cachedWindowWidth;
    }

    int getWindowHeight() {
        return cachedWindowHeight;
    }

    int getMonitorRefreshRate(int /*monitor*/) {
        const SDL_DisplayMode* mode =
            SDL_GetCurrentDisplayMode(SDL_GetPrimaryDisplay());
        return mode ? (int)mode->refresh_rate : 60;
    }

    int getCurrentMonitor() {
        return 0;
    }

    float getFrameTime() {
        return deltaTime;
    }

    double getTime() {
        return SDL_GetTicksNS() / 1000000000.0;
    }

    SDL_Renderer* getRenderer() {
        return renderer;
    }

    SDL_Window* getWindow() {
        return window;
    }

    Camera& getCamera() {
        return mainCamera;
    }

    void updateCamera(float dt) {
        mainCamera.update(dt);
    }

} // namespace OpenWars::IO::Graphics