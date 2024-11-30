#pragma once

#include <SDL2/SDL.h>

#include "utils/Vec2.hpp"
#include "utils/Renderer.hpp"

class Entity {
public:
    Entity(Renderer& renderer, SDL_Rect hitbox);
    virtual ~Entity() = default;

    virtual void Update(float dt) = 0;
    virtual void Render() = 0;

    void ResetHitBox();
    const SDL_Rect& GetHitBox() const;
    Vec2 GetPosition() const;

protected:
    Renderer& renderer_;
    SDL_Rect hitbox_;
    const SDL_Rect starting_hitbox_;
};
