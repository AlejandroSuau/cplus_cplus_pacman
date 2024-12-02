#pragma once

#include <SDL2/SDL.h>

#include "utils/Entity.hpp"
#include "utils/Vec2.hpp"

#include "GameMap.hpp"

// Order matters because it helps to rotate the asset while moving.
enum class EDirection {
    LEFT = 0,
    UP = 1,
    RIGHT = 2,
    DOWN = 3
};

class EntityMovable : public Entity {
public:
    EntityMovable(
        Renderer& renderer,
        SDL_FRect renderer_rect,
        const GameMap& game_map,
        float velocity,
        EDirection direction,
        float hitbox_scale = 1.f);
    ~EntityMovable() = default;

    virtual bool Step(float dt);
    bool StepHitBox(float dt, SDL_FRect& hitbox, EDirection direction) const;
    void StepIntoAllowedRandomDirection(float dt);
    void StepToTarget(float dt, Vec2<float> target_coords);
    bool TryToStep(float dt, EDirection direction);
    void AdjustPosition(Vec2<float> target_coords);
    void SetDirectionByTarget(Vec2<float> target_coords);

    void ReverseDirection();
    EDirection GetOppositeDirection();
    Vec2<int> GetDirectionVector() const;
    Vec2<int> GetDirectionVector(EDirection direction) const;

protected:
    const GameMap& game_map_;
    float velocity_;
    const EDirection starting_direction_;
    EDirection direction_;
    bool is_moving_between_tiles_;

    bool IsMovementAllowed(const SDL_FRect& moved_rect) const;
};