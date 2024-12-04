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
        SDL_FRect renderer_rect = GetRendererRect();
        StepRect(dt, renderer_rect, dir);
        return !IsMovementAllowed(renderer_rect);
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
    SDL_FRect renderer_rect = GetRendererRect();
    const bool did_step = StepRect(dt, renderer_rect, direction_);
    UpdatePosition({renderer_rect.x, renderer_rect.y});
    return did_step;
}

bool EntityMovable::StepRect(float dt, SDL_FRect& rect, EDirection direction) const {
    const auto delta = velocity_ * dt;
    const auto dir_vector = GetDirectionVector(direction);
    rect.x += delta * dir_vector.x;
    rect.y += delta * dir_vector.y;
    
    return true;
}

bool EntityMovable::DidReachCellCenter() const {
    const auto center_rect = GetCenterPosition();
    const auto center_cell = game_map_.FromCoordsToCenterCellCoords(center_rect);
    const auto tolerance = GetCellCenterTolerance();
    return (fabs(center_rect.x - center_cell.x) <= tolerance &&
            fabs(center_rect.y - center_cell.y) <= tolerance);
}

float EntityMovable::GetCellCenterTolerance() const {
    return (game_map_.GetCellSizeFloat() / 8.f);
}

bool EntityMovable::IsMovableDirection(EDirection direction) const {
    const auto center_rect = GetCenterPosition();
    if (!game_map_.AreCoordsInsideBoundaries(center_rect)) return false;

    const auto center_cell = game_map_.FromCoordsToCenterCellCoords(center_rect);
    const auto dir_vector = GetDirectionVector(direction);       
    const auto tolerance = GetCellCenterTolerance();
    
    bool did_pass_cell_center = (
        (dir_vector.x != 0 && fabs(center_rect.x - center_cell.x) < tolerance) ||
        (dir_vector.y != 0 && fabs(center_rect.y - center_cell.y) < tolerance));

    auto col_row = game_map_.FromCoordsToColRow(center_rect);
    if (did_pass_cell_center) {
        col_row += dir_vector;
    }

    return game_map_.AreColRowWalkable(col_row);
}

void EntityMovable::AdjustPosition(Vec2<float> target_coords) {
    /*SDL_FRect new_hitbox = GetHitBox();
    const auto dir_vector = GetDirectionVector();
    if ((dir_vector.x > 0 && new_hitbox.x >= target_coords.x) ||
        (dir_vector.x < 0 && new_hitbox.x <= target_coords.x)) {
        new_hitbox.x = target_coords.x;
    }
    
    if ((dir_vector.y > 0 && new_hitbox.y >= target_coords.y) ||
        (dir_vector.y < 0 && new_hitbox.y <= target_coords.y)) {
        new_hitbox.y = target_coords.y;
    }

    UpdatePosition({new_hitbox.x, new_hitbox.y});*/
}

Vec2<int> EntityMovable::GetDirectionVector() const {
    return GetDirectionVector(direction_);
}

bool EntityMovable::TryToStep(float dt, EDirection direction) {
    SDL_FRect renderer_rect = GetRendererRect();
    StepRect(dt, renderer_rect, direction);
    if (IsMovementAllowed(renderer_rect)) {
        direction_ = direction;
        UpdatePosition({renderer_rect.x, renderer_rect.y});
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