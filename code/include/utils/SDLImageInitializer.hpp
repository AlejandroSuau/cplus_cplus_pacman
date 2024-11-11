#pragma once

#include <SDL2/SDL.h>

#include <stdexcept>

class SDLInitializer {
public:
    SDLInitializer() {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            SDL_Log("Failed to init SDL: %s", SDL_GetError());
            throw std::runtime_error("Failed to init SDL");
        }
    }
    
    ~SDLInitializer() { SDL_Quit(); }
};