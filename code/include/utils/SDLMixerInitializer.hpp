#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include <stdexcept>

// 44100: Frecuencia de muestreo (Hz).
// MIX_DEFAULT_FORMAT: Formato de audio predeterminado.
// 2: Número de canales de audio (2 para sonido estéreo).
// 2048: Tamaño del búfer de audio. Puedes ajustar este valor según tus necesidades.

class SDLMixerInitializer {
public:
    SDLMixerInitializer() {
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            SDL_Log("SDL_mixer could not initialize! SDL_mixer Error: %s", Mix_GetError());
        }
    }
    
    ~SDLMixerInitializer() { Mix_CloseAudio(); }
};
