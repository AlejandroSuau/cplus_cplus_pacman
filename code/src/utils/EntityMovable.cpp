#include "utils/EntityMovable.hpp"

#include <algorithm>
#include <array>

EntityMovable::EntityMovable(
    Renderer& renderer,
    SDL_Rect hitbox,
    const GameMap& game_map,
    float velocity,
    EDirection direction)
    : Entity(renderer, hitbox)
    , game_map_(game_map)
    , velocity_(velocity)
    , starting_direction_(direction)
    , direction_(direction)
    , is_moving_between_tiles_(false) {}

bool EntityMovable::Step(float dt) {
    const auto delta = static_cast<int>(velocity_ * dt);
    const auto dir_vector = GetDirectionVector();
    hitbox_.x += delta * dir_vector.x;
    hitbox_.y += delta * dir_vector.y;
    
    return true;
}

void EntityMovable::StepToTarget(float dt, Vec2 target) {

}

Vec2 EntityMovable::GetDirectionVector() const {
    return GetDirectionVector(direction_);
}

bool EntityMovable::TryToStep(float dt, EDirection direction) {
    SDL_Rect next_hitbox = hitbox_;
    const auto dir_vector = GetDirectionVector(direction);
    const auto delta = static_cast<int>(velocity_ * dt);
    next_hitbox.x += delta * dir_vector.x;
    next_hitbox.y += delta * dir_vector.y;
    if (IsMovementAllowed(next_hitbox)) {
        hitbox_ = next_hitbox;
        direction_ = direction;
        return true;
    }

    return false;
}

Vec2 EntityMovable::GetDirectionVector(EDirection direction) const {
    switch(direction) {
        case EDirection::DOWN:  return { 0,  1};
        case EDirection::UP:    return { 0, -1};
        case EDirection::LEFT:  return {-1,  0};
        case EDirection::RIGHT: return { 1,  0};
        default:                return {};
    }
}

bool EntityMovable::IsMovementAllowed(const SDL_Rect& moved_rect) const {
    const std::array<Vec2, 4> rect_points {{
        {moved_rect.x, moved_rect.y},
        {moved_rect.x, moved_rect.y + moved_rect.h},
        {moved_rect.x + moved_rect.w, moved_rect.y},
        {moved_rect.x + moved_rect.w, moved_rect.y + moved_rect.h}}};
    auto is_coord_walkable = [&](const auto& coords) {
        return game_map_.AreCoordsWalkable(coords.x, coords.y);
    };
    return std::all_of(rect_points.cbegin(), rect_points.cend(), is_coord_walkable);
}

void EntityMovable::ReverseDirection() {
    direction_ = GetOppositeDirection();
}

EDirection EntityMovable::GetOppositeDirection() {
    switch(direction_) {
        default:
        case EDirection::UP:    return EDirection::DOWN;
        case EDirection::DOWN:  return EDirection::UP;
        case EDirection::RIGHT: return EDirection::LEFT;
        case EDirection::LEFT:  return EDirection::RIGHT;
    }
}