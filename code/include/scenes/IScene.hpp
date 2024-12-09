#pragma once

#include <SDL2/SDL.h>

class IScene {
public:
    virtual ~IScene() = default;
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual void OnKeyEvent(const SDL_Event& event) {}
};