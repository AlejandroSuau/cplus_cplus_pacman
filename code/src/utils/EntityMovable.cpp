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
    const auto center_position = GetCenterPosition();
    Vec2<float> diff = target_coords - center_position;
    if (std::abs(diff.x) > std::abs(diff.y)) {
        direction_ = (diff.x > 0) ? EDirection::RIGHT : EDirection::LEFT;
    } else {
        direction_ = (diff.y > 0) ? EDirection::DOWN : EDirection::UP;
    }
}

bool EntityMovable::IsOrthogonalTurn(EDirection next_direction) const {
    return (direction_ == EDirection::UP || direction_ == EDirection::DOWN) &&
           (next_direction == EDirection::LEFT || next_direction == EDirection::RIGHT) || 
           (direction_ == EDirection::LEFT || direction_ == EDirection::RIGHT) && 
           (next_direction == EDirection::UP || next_direction == EDirection::DOWN);
}

void EntityMovable::Step(float dt) {
    const SDL_FRect renderer_rect = GetRendererRect();
    const auto delta = velocity_ * dt;

    const auto dir_vector = GetDirectionVector();
    Vec2<float> new_position {renderer_rect.x, renderer_rect.y};
    new_position.x += delta * dir_vector.x;
    new_position.y += delta * dir_vector.y;
    UpdatePosition(new_position);

    if (dir_vector.y != 0) {
        CenterAxisX();
    } else if (dir_vector.x != 0) {
        CenterAxisY();
    }
}

bool EntityMovable::DidReachCellCenter() const {
    const auto center_rect = GetCenterPosition();
    const auto center_cell = game_map_.FromCoordsToCenterCellCoords(center_rect);
    const auto tolerance = GetCellCenterTolerance();
    return (fabs(center_rect.x - center_cell.x) <= tolerance &&
            fabs(center_rect.y - center_cell.y) <= tolerance);
}

float EntityMovable::GetCellCenterTolerance() const {
    // If velocity increases it will need a higher number of tolerancy.
    return (game_map_.GetCellSizeFloat() / 16.f);
}

bool EntityMovable::IsMovableDirection(EDirection direction) const {
    const auto center_rect = GetCenterPosition();
    if (!game_map_.AreCoordsInsideBoundaries(center_rect)) {
        return false;
    }

    static const Vec2<int> kHousesDoorColRow {8, 6};
    const auto& cell = game_map_.GetCell(center_rect);
    auto col_row = game_map_.FromCoordsToColRow(cell.center);    
    if (kHousesDoorColRow == col_row && direction == EDirection::DOWN) {
        return false;
    }

    const auto dir_vector = GetDirectionVector(direction);       
    const auto tolerance = GetCellCenterTolerance();
    
    bool did_pass_cell_center = (
        (dir_vector.x != 0 && fabs(center_rect.x - cell.center.x) < tolerance) ||
        (dir_vector.y != 0 && fabs(center_rect.y - cell.center.y) < tolerance));
    if (did_pass_cell_center) {
        col_row += dir_vector;
    }

    return game_map_.AreColRowWalkable(col_row);
}

void EntityMovable::CenterAxisX() {
    CenterAxis(true, false);
}

void EntityMovable::CenterAxisY() {
    CenterAxis(false, true);
}

void EntityMovable::CenterAxis(bool center_x, bool center_y) {
    const auto center_rect = GetCenterPosition();
    const auto center_cell = game_map_.FromCoordsToCenterCellCoords(center_rect);
    
    const auto rect = GetRendererRect();
    Vec2<float> pos {rect.x, rect.y};
    if (center_x) pos.x = center_cell.x - rect.w / 2.f;
    if (center_y) pos.y = center_cell.y - rect.h / 2.f;

    UpdatePosition(pos);
}

Vec2<int> EntityMovable::GetDirectionVector() const {
    return GetDirectionVector(direction_);
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

void EntityMovable::SetVelocity(float velocity) {
    velocity_ = velocity;
}

bool EntityMovable::IsAtHousesDoorCell() const {
    static const Vec2<int> kHousesDoorColRow {8, 6};
    const auto& center_position = GetCenterPosition();
    const auto col_row = game_map_.FromCoordsToColRow(center_position);
    return (kHousesDoorColRow == col_row);
}