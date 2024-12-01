#include "utils/EntityMovable.hpp"

#include <algorithm>
#include <array>
#include <random>

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

void EntityMovable::SetDirectionByTarget(Vec2 target_coords) {
    if (hitbox_.x < target_coords.x) {
        direction_ = EDirection::RIGHT;
    } else if (hitbox_.x > target_coords.x) {
        direction_ = EDirection::LEFT;
    } else if (hitbox_.y < target_coords.y) {
        direction_ = EDirection::DOWN;
    } else if (hitbox_.y > target_coords.y) {
        direction_ = EDirection::UP;
    }
}

void EntityMovable::StepIntoAllowedRandomDirection(float dt) {
    std::array directions {
        EDirection::LEFT,
        EDirection::UP,
        EDirection::DOWN,
        EDirection::RIGHT
    };

    auto directions_end = std::remove(
        directions.begin(), directions.end(), GetOppositeDirection());
    
    const int delta = static_cast<int>(velocity_ * dt);
    auto is_prohibited_movement = [&](EDirection dir) {
        SDL_Rect next_hitbox = hitbox_;
        StepHitBox(dt, next_hitbox, dir);
        return !IsMovementAllowed(next_hitbox);
    };

    directions_end = std::remove_if(directions.begin(), directions_end, is_prohibited_movement);
    if (std::distance(directions.begin(), directions_end) > 1) {
        static thread_local std::mt19937 rng{std::random_device{}()};
        std::ranges::shuffle(directions.begin(), directions_end, rng);
    }

    direction_ = *directions.begin();
    Step(dt);
}

void EntityMovable::StepToTarget(float dt, Vec2 target_coords) {
    Step(dt);
    AdjustPosition(target_coords);
}

bool EntityMovable::Step(float dt) {
    return StepHitBox(dt, hitbox_, direction_);
}

bool EntityMovable::StepHitBox(float dt, SDL_Rect& hitbox, EDirection direction) const {
    const auto delta = static_cast<int>(velocity_ * dt);
    const auto dir_vector = GetDirectionVector(direction);
    hitbox.x += delta * dir_vector.x;
    hitbox.y += delta * dir_vector.y;
    
    return true;
}

void EntityMovable::AdjustPosition(Vec2 target_coords) {
    const auto dir_vector = GetDirectionVector();
    if ((dir_vector.x > 0 && hitbox_.x >= target_coords.x) ||
        (dir_vector.x < 0 && hitbox_.x <= target_coords.x)) {
        hitbox_.x = target_coords.x;
    }
    
    if ((dir_vector.y > 0 && hitbox_.y >= target_coords.y) ||
        (dir_vector.y < 0 && hitbox_.y <= target_coords.y)) {
        hitbox_.y = target_coords.y;
    }
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