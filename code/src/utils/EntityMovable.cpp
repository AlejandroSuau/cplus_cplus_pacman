#include "utils/EntityMovable.hpp"

#include <algorithm>
#include <array>
#include <random>

EntityMovable::EntityMovable(
    Renderer& renderer,
    SDL_FRect renderer_rect,
    const GameMap& game_map,
    float velocity,
    EDirection direction,
    float hitbox_scale)
    : Entity(renderer, renderer_rect, hitbox_scale)
    , game_map_(game_map)
    , velocity_(velocity)
    , starting_direction_(direction)
    , direction_(direction)
    , is_moving_between_tiles_(false) {}

void EntityMovable::SetDirectionByTarget(Vec2<float> target_coords) {
    const auto& hitbox = GetHitBox();
    if (hitbox.x < target_coords.x) {
        direction_ = EDirection::RIGHT;
    } else if (hitbox.x > target_coords.x) {
        direction_ = EDirection::LEFT;
    } else if (hitbox.y < target_coords.y) {
        direction_ = EDirection::DOWN;
    } else if (hitbox.y > target_coords.y) {
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
    
    auto is_prohibited_movement = [&](EDirection dir) {
        SDL_FRect next_hitbox = GetHitBox();
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

void EntityMovable::StepToTarget(float dt, Vec2<float> target_coords) {
    Step(dt);
    AdjustPosition(target_coords);
}

bool EntityMovable::Step(float dt) {
    SDL_FRect hitbox = GetRendererRect();//GetHitBox();
    const bool did_step = StepHitBox(dt, hitbox, direction_);
    UpdatePosition({hitbox.x, hitbox.y});
    return did_step;
}

bool EntityMovable::StepHitBox(float dt, SDL_FRect& hitbox, EDirection direction) const {
    const auto delta = velocity_ * dt;
    const auto dir_vector = GetDirectionVector(direction);
    hitbox.x += delta * dir_vector.x;
    hitbox.y += delta * dir_vector.y;
    
    return true;
}

void EntityMovable::AdjustPosition(Vec2<float> target_coords) {
    SDL_FRect new_hitbox = GetHitBox();
    const auto dir_vector = GetDirectionVector();
    if ((dir_vector.x > 0 && new_hitbox.x >= target_coords.x) ||
        (dir_vector.x < 0 && new_hitbox.x <= target_coords.x)) {
        new_hitbox.x = target_coords.x;
    }
    
    if ((dir_vector.y > 0 && new_hitbox.y >= target_coords.y) ||
        (dir_vector.y < 0 && new_hitbox.y <= target_coords.y)) {
        new_hitbox.y = target_coords.y;
    }

    UpdatePosition({new_hitbox.x, new_hitbox.y});
}

Vec2<int> EntityMovable::GetDirectionVector() const {
    return GetDirectionVector(direction_);
}

bool EntityMovable::TryToStep(float dt, EDirection direction) {
    SDL_FRect next_hitbox = GetHitBox();
    StepHitBox(dt, next_hitbox, direction);
    if (IsMovementAllowed(next_hitbox)) {
        direction_ = direction;
        UpdatePosition({next_hitbox.x, next_hitbox.y});
        return true;
    }

    return false;
}

Vec2<int> EntityMovable::GetDirectionVector(EDirection direction) const {
    switch(direction) {
        case EDirection::DOWN:  return { 0,  1};
        case EDirection::UP:    return { 0, -1};
        case EDirection::LEFT:  return {-1,  0};
        case EDirection::RIGHT: return { 1,  0};
        default:                return {};
    }
}

bool EntityMovable::IsMovementAllowed(const SDL_FRect& moved_rect) const {
    const std::array<Vec2<float>, 4> rect_points {{
        {moved_rect.x, moved_rect.y},
        {moved_rect.x, moved_rect.y + moved_rect.h},
        {moved_rect.x + moved_rect.w, moved_rect.y},
        {moved_rect.x + moved_rect.w, moved_rect.y + moved_rect.h}}};
    auto is_coord_walkable = [&](const auto& coords) {
        return game_map_.AreCoordsWalkable(coords);
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