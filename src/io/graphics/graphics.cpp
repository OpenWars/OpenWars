#include "graphics.hpp"
#include "../../core/colors.hpp"
#include "../../core/drawing/text.hpp"
#include <SDL3_ttf/SDL_ttf.h>
#include <map>

namespace OpenWars::IO::Graphics {
    static SDL_Window* window = nullptr;
    static SDL_Renderer* renderer = nullptr;
    static Camera3D camera;
    static bool shouldCloseFlag = false;
    static uint64_t lastFrameTime = 0;
    static float deltaTime = 0.0f;
    static int frameCount = 0;
    static double lastFPSTime = 0.0;
    static int currentFPS = 0;

    void init(int vsync, bool multisampling) {
        if(!SDL_Init(SDL_INIT_VIDEO)) {
            SDL_Log("SDL init failed: %s", SDL_GetError());
            return;
        }

        if(!TTF_Init()) {
            SDL_Log("TTF init failed: %s", SDL_GetError());
            return;
        }

        uint32_t flags = SDL_WINDOW_RESIZABLE;
        window = SDL_CreateWindow("OpenWars Engine", 1024, 512, flags);
        if(!window) {
            SDL_Log("Window creation failed: %s", SDL_GetError());
            return;
        }

        renderer = SDL_CreateRenderer(window, nullptr);
        if(!renderer) {
            SDL_Log("Renderer creation failed: %s", SDL_GetError());
            return;
        }

        SDL_SetRenderVSync(renderer, vsync);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        camera =
            {Vector3{0, 0, 0}, Vector3{0, 0, 0}, Vector3{0, 1, 0}, 45.0f, 0};

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
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_EVENT_QUIT) {
                shouldCloseFlag = true;
            }
        }
        return shouldCloseFlag;
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

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    void beginAttached() {
        // TODO (sdl3_gpu?)
    }

    void endAttached() {
        // TODO (same as beginAttached)
    }

    Camera3D getCamera() {
        return camera;
    }

    void swapBuffers() {
        SDL_RenderPresent(renderer);
    }

    void clearBackground(Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderClear(renderer);
    }

    int getScreenWidth() {
        int w = 0;
        if(window)
            SDL_GetWindowSize(window, &w, nullptr);
        return w;
    }

    int getScreenHeight() {
        int h = 0;
        if(window)
            SDL_GetWindowSize(window, nullptr, &h);
        return h;
    }

    int getMonitorRefreshRate(int monitor) {
        const SDL_DisplayMode* mode =
            SDL_GetCurrentDisplayMode(SDL_GetPrimaryDisplay());
        return mode ? (int)mode->refresh_rate : 60;
    }

    int getCurrentMonitor() {
        return 0; // sdl uses display IDs, simplify for now
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
} // namespace OpenWars::IO::Graphics