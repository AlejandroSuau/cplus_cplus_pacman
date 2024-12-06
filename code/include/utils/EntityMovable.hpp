#pragma once

#include <SDL2/SDL.h>

#include "utils/Entity.hpp"
#include "utils/Vec2.hpp"

#include "GameMap.hpp"

// Order matters because it helps to rotate the asset while moving.
enum class EDirection {
    RIGHT = 0,
    DOWN = 1,
    LEFT = 2,
    UP = 3
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

    void Step(float dt);
    void StepIntoAllowedRandomDirection(float dt);
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

    bool IsAtHousesDoorCell() const;

    bool DidReachCellCenter() const;
    float GetCellCenterTolerance() const;
    bool IsMovableDirection(EDirection direction) const;
    bool IsOrthogonalTurn(EDirection next_direction) const;
    void CenterAxisX();
    void CenterAxisY();
    void CenterAxis(bool center_x = true, bool center_y = true);
};