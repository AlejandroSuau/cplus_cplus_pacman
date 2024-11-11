#pragma once

#include <SDL2/SDL_image.h>

#include <stdexcept>

class SDLImageInitializer {
public:
    SDLImageInitializer() {
        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            SDL_Log("Failed to init SDL: %s", SDL_GetError());
            throw std::runtime_error("Failed to init Image SDL");
        }
    }
    
    ~SDLImageInitializer() { IMG_Quit(); }
};