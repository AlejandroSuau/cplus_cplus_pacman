#pragma once

#include <SDL2/SDL.h>

bool AreColliding(const SDL_Rect& a, const SDL_Rect& b);
bool AreColliding(const SDL_FRect& a, const SDL_FRect& b);