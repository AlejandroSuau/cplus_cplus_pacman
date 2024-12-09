#pragma once

#include <SDL2/SDL.h>

#include "utils/Vec2.hpp"

bool AreColliding(const SDL_Rect& a, const SDL_Rect& b);
bool AreColliding(const SDL_FRect& a, const SDL_FRect& b);

bool IsPointInsideRect(const Vec2<int>& point, const SDL_Rect& rect);
bool IsPointInsideRect(const Vec2<float>& point, const SDL_FRect& rect);
