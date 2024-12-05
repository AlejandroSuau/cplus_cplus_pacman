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
        bool is_movable = IsMovableDirection(dir); // y si es ortogonal, tambien did reach cell center
        return !is_movable; 
    };

    directions_end = std::remove_if(directions.begin(), directions_end, is_prohibited_movement);
    if (std::distance(directions.begin(), directions_end) > 1) {
        static thread_local std::mt19937 rng{std::random_device{}()};
        std::ranges::shuffle(directions.begin(), directions_end, rng);
    }

    // PROBLEMa: cambia mas de una vez en la misma intersecc
    const auto dir = *directions.begin();
    if (IsOrthogonalTurn(dir) && DidReachCellCenter()) {
        SDL_Log("------------------------------------------------------");
        SDL_Log("Was going direction: %d", static_cast<int>(direction_));
        direction_ = dir;
        SDL_Log("NEW DIRECTION: %d", static_cast<int>(direction_));
        SDL_Log("------------------------------------------------------");
    } else {
        SDL_Log("No changing");
    }
    Step(dt);
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