#pragma once

#include <SDL2/SDL_ttf.h>

#include <stdexcept>

class SDLTTFInitializer {
public:
    SDLTTFInitializer() {
        if (TTF_Init() == -1) {
            SDL_Log("Failed to init SDL TTF: %s", SDL_GetError());
            throw std::runtime_error("Failed to init SDL TTF");
        }
    }
    
    ~SDLTTFInitializer() { TTF_Quit(); }
};