#pragma once

#include <SDL2/SDL.h>

class CountdownTimer {
public:
    CountdownTimer(Uint64 interval) 
        : interval_(interval)
        , last_time_(SDL_GetTicks64()) {}

    void Start() {
        last_time_ = SDL_GetTicks64();
    }

    bool HasElapsed() {
        Uint64 current_time = SDL_GetTicks64();
        if (current_time - last_time_ >= interval_) {
            last_time_ = current_time;
            return true;
        }
        return false;
    }

private:
    Uint64 interval_;
    Uint64 last_time_;
};