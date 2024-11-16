#include "utils/Collisions.hpp"

bool AreColliding(const SDL_Rect& a, const SDL_Rect& b) {
    return !(a.x + a.w <= b.x || // El lado derecho de este rectángulo está a la izquierda del otro
             a.x >= b.x + b.w || // El lado izquierdo de este rectángulo está a la derecha del otro
             a.y + a.h <= b.y || // El lado inferior de este rectángulo está arriba del otro
             a.y >= b.y + b.h); // El lado superior de este rectángulo está abajo del otro
}

bool AreColliding(const SDL_FRect& a, const SDL_FRect& b) {
    return !(a.x + a.w <= b.x || // El lado derecho de este rectángulo está a la izquierda del otro
             a.x >= b.x + b.w || // El lado izquierdo de este rectángulo está a la derecha del otro
             a.y + a.h <= b.y || // El lado inferior de este rectángulo está arriba del otro
             a.y >= b.y + b.h); // El lado superior de este rectángulo está abajo del otro
}