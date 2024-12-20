#pragma once

#include <SDL2/SDL.h>

#include "utils/Vec2.hpp"
#include "utils/Renderer.hpp"

class GameScene;
class Entity {
public:
    Entity(Renderer& renderer, SDL_FRect renderer_rect, float hitbox_scale = 1.f);
    virtual ~Entity() = default;

    virtual void Update(float dt, GameScene* game_scene = nullptr) = 0;
    virtual void Render() = 0;

    void Reset();
    void UpdatePosition(Vec2<float> new_coords);

    const SDL_FRect& GetRendererRect() const;
    const SDL_FRect& GetHitBox() const;
    Vec2<float> GetPosition() const;
    Vec2<float> GetCenterPosition() const;

protected:
    Renderer& renderer_;
    const float hitbox_scale_;
    const SDL_FRect starting_renderer_rect_;

private:
    SDL_FRect renderer_rect_;
    SDL_FRect hitbox_;

    void UpdateHitBox();
};
