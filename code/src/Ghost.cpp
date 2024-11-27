#include "Ghost.hpp"

#include "Constants.hpp"
#include "Game.hpp"

#include <algorithm>
#include <array>
#include <random>

Ghost::Ghost(
    TextureManager& texture_manager,
    const GameMap& game_map,
    std::string name,
    EType type,
    int x,
    int y,
    EMovingDirection direction,
    PathfindingPattern pathfinding_pattern)
    : texture_manager_(texture_manager)
    , game_map_(game_map)
    , name_(name)
    , type_(type)
    , hitbox_(x, y, 31, 31)
    , direction_(direction)
    , patfinder_pattern_(pathfinding_pattern)
    , state_(EState::HOUSING)
    , path_index_(0)
    , path_step_timer_(0.5f)
    , animation_timer_(0.1f)
    , sprite_index_(0)
    , sprites_count_(2) {
    
    sprite_sheet_ = texture_manager_.LoadTexture(kAssetsFolderImages + "spritesheet.png");
}

void Ghost::FindPath(Game& game) {
    const auto [row, col] = game_map_.FromCoordsToRowCol(hitbox_.x, hitbox_.y);
    path_step_timer_.Restart();
    path_index_ = 1;
    path_ = patfinder_pattern_({col, row}, game);
}

void Ghost::Update(float dt) {
    animation_timer_.Update(dt);
    if (animation_timer_.DidFinish()) {
        sprite_index_ = (sprite_index_ + 1) % sprites_count_;
    }

    switch(state_) {
        case EState::HOUSING:    UpdateStateHouse(dt);       break;
        case EState::FRIGHTENED: UpdateStateFrightened(dt);  break;
        case EState::CHASING:    UpdateStateChasing(dt);     break;
    }
}

void Ghost::UpdateStateHouse(float dt) {
    timer_mode_house_.Update(dt);
    timer_mode_house_swap_direction_.Update(dt);
    const auto direction_vector = GetDirectionVector();
    const auto delta = static_cast<int>(GhostParameters::kVelocityStateHousing * dt);
    hitbox_.x += delta * direction_vector.x;
    hitbox_.y += delta * direction_vector.y;
    if (timer_mode_house_swap_direction_.DidFinish()) {
        ReverseDirection();
    }
    if (timer_mode_house_.DidFinish()) {
        state_ = EState::CHASING;
    }
}

void Ghost::UpdateStateChasing(float dt) {
    if (path_index_ >= path_.size()) return;

    const Vec2 position {hitbox_.x, hitbox_.y};
    const auto [target_row, target_col] = path_[path_index_];
    const auto [target_x, target_y] = game_map_.FromRowColToCoords(target_row, target_col);

    if (position.x < target_x) {
        direction_ = EMovingDirection::RIGHT;
    } else if (position.x > target_x) {
        direction_ = EMovingDirection::LEFT;
    } else if (position.y < target_y) {
        direction_ = EMovingDirection::DOWN;
    } else if (position.y > target_y) {
        direction_ = EMovingDirection::UP;
    } else {
        ++path_index_;
        return;
    }

    const auto delta = static_cast<int>(GhostParameters::kVelocityStateChasing * dt);
    const auto dir_vector = GetDirectionVector();
    hitbox_.x += delta * dir_vector.x;
    hitbox_.y += delta * dir_vector.y;
}

void Ghost::UpdateStateFrightened(float dt) {
    const int delta = static_cast<int>(PlayerParameters::kVelocity * dt);
    std::array directions {
        EMovingDirection::LEFT,
        EMovingDirection::UP,
        EMovingDirection::DOWN,
        EMovingDirection::RIGHT
    };

    auto directions_end = std::remove(
        directions.begin(), directions.end(), GetOppositeDirection());
    auto is_prohibited_movement = [&](EMovingDirection dir) {
        SDL_Rect next_hitbox_ = hitbox_;
        const auto dir_vector = GetDirectionVector(dir);
        next_hitbox_.x += delta * dir_vector.x;
        next_hitbox_.y += delta * dir_vector.y;
        return !IsMovementAllowed(next_hitbox_);
    };

    directions_end = std::remove_if(directions.begin(), directions_end, is_prohibited_movement);
    if (std::distance(directions.begin(), directions_end) > 1) {
        static thread_local std::mt19937 rng{std::random_device{}()};
        std::ranges::shuffle(directions.begin(), directions_end, rng);
    }

    direction_ = *directions.begin();
    const auto direction_vector = GetDirectionVector();
    hitbox_.x += delta * direction_vector.x; 
    hitbox_.y += delta * direction_vector.y;
}

bool Ghost::TryToMove(Vec2 delta_movement) {
    const auto dir_vector = GetDirectionVector();
    SDL_Rect next_hitbox = hitbox_;
    next_hitbox.x += delta_movement.x * dir_vector.x;
    next_hitbox.y += delta_movement.y * dir_vector.y;
    if (IsMovementAllowed(next_hitbox)) {
        hitbox_ = next_hitbox;
        return true;
    }

    return false;
}

bool Ghost::IsMovementAllowed(SDL_Rect moved_rect) const {
    const std::array<std::pair<int, int>, 4> rect_points {{
        {moved_rect.x, moved_rect.y},
        {moved_rect.x, moved_rect.y + moved_rect.h},
        {moved_rect.x + moved_rect.w, moved_rect.y},
        {moved_rect.x + moved_rect.w, moved_rect.y + moved_rect.h}}};
    auto is_coord_walkable = [&](const auto& coord_pair) {
        return game_map_.AreCoordsWalkable(coord_pair.first, coord_pair.second);
    };
    return std::all_of(rect_points.cbegin(), rect_points.cend(), is_coord_walkable);
}

void Ghost::ActivateFrightenedMode() {
    state_ = EState::FRIGHTENED;
    // render el otro
    // comenzar el tempor
    // Si han pasado X segundos, cambiamos la imagen durante 1s. luego vuelve y luego vuelve a hacerlo
    // despues termina vulnerability

    // igual pero 163
}

void Ghost::Render(SDL_Renderer& renderer) {
    RenderPath(renderer);
    auto src_r = GetSourceRect();
    SDL_RenderCopyEx(&renderer, sprite_sheet_, &src_r, &hitbox_, 0, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
}

void Ghost::RenderPath(SDL_Renderer& renderer) {
    SDL_SetRenderDrawColor(&renderer, 200, 200, 200, 50);
    const auto cell_size = game_map_.GetCellSizeInt();
    for (const auto& [row, col] : path_) {
        const auto& cell = game_map_.GetCell(row, col);
        SDL_Rect r {cell.x, cell.y, cell_size, cell_size};
        SDL_RenderFillRect(&renderer, &r);
    }
}

SDL_Rect Ghost::GetSourceRect() const {
    using namespace SpriteSheet;
    const auto dir = static_cast<int>(direction_);
    const int x = kStartingX +
        ((kPadding + kWidth) * GhostSprite::kAnimationCount) * dir +
        ((kPadding + kWidth) * sprite_index_);
    const int y = GhostSprite::kStartingY + (kPadding + kHeight) * static_cast<int>(type_);
    return SDL_Rect {x, y, kWidth, kHeight};
}

const std::string_view Ghost::GetName() const {
    return name_;
}

Vec2 Ghost::GetDirectionVector() const {
    return GetDirectionVector(direction_);
}

Vec2 Ghost::GetDirectionVector(EMovingDirection direction) const {
    switch(direction) {
        default:
        case EMovingDirection::DOWN:  return { 0,  1};
        case EMovingDirection::UP:    return { 0, -1};
        case EMovingDirection::LEFT:  return {-1,  0};
        case EMovingDirection::RIGHT: return { 1,  0};
    }
}

bool Ghost::IsOnChasingState() const {
    return (state_ == EState::CHASING);
}

Vec2 Ghost::GetPosition() const {
    return {hitbox_.x, hitbox_.y};
}

void Ghost::ReverseDirection() {
    direction_ = GetOppositeDirection();
}

Ghost::EMovingDirection Ghost::GetOppositeDirection() const {
    switch(direction_) {
        default:
        case EMovingDirection::UP:    return EMovingDirection::DOWN;
        case EMovingDirection::DOWN:  return EMovingDirection::UP;
        case EMovingDirection::RIGHT: return EMovingDirection::LEFT;
        case EMovingDirection::LEFT:  return EMovingDirection::RIGHT;
    }
}
