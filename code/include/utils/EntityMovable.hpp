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
        SDL_Rect hitbox,
        const GameMap& game_map,
        float velocity,
        EDirection direction);
    ~EntityMovable() = default;

    virtual bool Step(float dt);
    bool StepHitBox(float dt, SDL_Rect& hitbox, EDirection direction) const;

    void StepIntoAllowedRandomDirection(float dt);
    void StepToTarget(float dt, Vec2 target_coords);
    bool TryToStep(float dt, EDirection direction);
    void AdjustPosition(Vec2 target_coords);
    void SetDirectionByTarget(Vec2 target_coords);

    void ReverseDirection();
    EDirection GetOppositeDirection();
    Vec2 GetDirectionVector() const;
    Vec2 GetDirectionVector(EDirection direction) const;

protected:
    const GameMap& game_map_;
    float velocity_;
    const EDirection starting_direction_;
    EDirection direction_;
    bool is_moving_between_tiles_;

    bool IsMovementAllowed(const SDL_Rect& moved_rect) const;
};