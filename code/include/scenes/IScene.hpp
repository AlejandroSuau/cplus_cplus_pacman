#pragma once

#include <SDL2/SDL.h>

#include "utils/Vec2.hpp"

enum class EEventMouse {
    DOWN,
    UP,
    MOTION
};

enum class EEventKeyboard {
    DOWN,
    UP
};

class Game;
class IScene {
public:
    virtual ~IScene() = default;
    
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual void OnEvent(const SDL_Event& event, Game* game) = 0;
};