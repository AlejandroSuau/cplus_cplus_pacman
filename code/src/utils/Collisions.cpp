#include "utils/Collisions.hpp"

bool AreColliding(const SDL_Rect& a, const SDL_Rect& b) {
    return !(a.x + a.w <= b.x ||
             a.x >= b.x + b.w ||
             a.y + a.h <= b.y ||
             a.y >= b.y + b.h);
}

bool AreColliding(const SDL_FRect& a, const SDL_FRect& b) {
    return !(a.x + a.w <= b.x || 
             a.x >= b.x + b.w || 
             a.y + a.h <= b.y || 
             a.y >= b.y + b.h);
}

bool IsPointInsideRect(const Vec2<int>& point, const SDL_Rect& rect) {
    return point.x >= rect.x &&
           point.x < (rect.x + rect.w) &&
           point.y >= rect.y &&
           point.y < (rect.y + rect.h);
}

bool IsPointInsideRect(const Vec2<float>& point, const SDL_FRect& rect) {
    return point.x >= rect.x &&
           point.x < (rect.x + rect.w) &&
           point.y >= rect.y &&
           point.y < (rect.y + rect.h);
}
