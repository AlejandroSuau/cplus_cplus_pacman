#pragma once

#include <SDL2/SDL.h>

#include "utils/Vec2.hpp"
#include "utils/Renderer.hpp"

class Entity {
public:
    Entity(Renderer& renderer, SDL_FRect renderer_rect, float hitbox_scale = 1.f);
    virtual ~Entity() = default;

    virtual void Update(float dt) = 0;
    virtual void Render() = 0;

    void ResetHitBox();
    const SDL_FRect& GetRendererRect() const;
    const SDL_FRect& GetHitBox() const;
    Vec2<float> GetPosition() const;
    void UpdatePosition(Vec2<float> new_coords);

protected:
    Renderer& renderer_;
    const float hitbox_scale_;
    const SDL_FRect starting_hitbox_;

private:
    SDL_FRect renderer_rect_;
    SDL_FRect hitbox_;

    void UpdateHitBox();
};
