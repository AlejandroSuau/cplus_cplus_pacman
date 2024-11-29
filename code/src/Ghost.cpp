#include "Ghost.hpp"

#include "Constants.hpp"
#include "Game.hpp"

#include <algorithm>
#include <array>
#include <random>
#include <cmath>

Ghost::Ghost(
    TextureManager& texture_manager,
    Pathfinder& pathfinder,
    const GameMap& game_map,
    std::string name,
    EType type,
    int x,
    int y,
    EMovingDirection direction,
    PathfindingPattern pathfinding_pattern)
    : texture_manager_(texture_manager)
    , pathfinder_(pathfinder)
    , game_map_(game_map)
    , name_(name)
    , type_(type)
    , hitbox_(x, y, 31, 31)
    , starting_position_(x, y)
    , direction_(direction)
    , starting_direction_(direction)
    , patfinder_pattern_(pathfinding_pattern)
    , state_(EState::HOUSING)
    , path_index_(0)
    , is_moving_between_tiles_(false)
    , animation_timer_(0.1f)
    , sprite_index_(0)
    , sprites_count_(2) {
    
    sprite_sheet_ = texture_manager_.LoadTexture(kAssetsFolderImages + "spritesheet.png");
}

void Ghost::Reset() {
    hitbox_.x = starting_position_.x;
    hitbox_.y = starting_position_.y;
    direction_ = starting_direction_;
    state_ = EState::HOUSING;
    is_moving_between_tiles_ = false;
    timer_mode_house_.Restart();
    timer_mode_house_swap_direction_.Restart();
    path_.clear();
}

void Ghost::FindPath(Game& game) {
    if (is_moving_between_tiles_) return;

    const auto [row, col] = game_map_.FromCoordsToRowCol(hitbox_.x, hitbox_.y);
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
        case EState::EYES:       UpdateStateEyes(dt);        break;
        default:                                             
        case EState::STOP:                                   break;
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

    is_moving_between_tiles_ = true;
    const auto [target_row, target_col] = path_[path_index_];
    const auto [target_x, target_y] = game_map_.FromRowColToCoords(
        path_[path_index_].first, path_[path_index_].second);

    if (hitbox_.x < target_x) {
        direction_ = EMovingDirection::RIGHT;
    } else if (hitbox_.x > target_x) {
        direction_ = EMovingDirection::LEFT;
    } else if (hitbox_.y < target_y) {
        direction_ = EMovingDirection::DOWN;
    } else if (hitbox_.y > target_y) {
        direction_ = EMovingDirection::UP;
    } else {
        is_moving_between_tiles_ = false;
        ++path_index_;
        hitbox_.x = target_x;
        hitbox_.y = target_y;
        return;
    }

    const auto delta = static_cast<int>(GhostParameters::kVelocityStateChasing * dt);
    const auto dir_vector = GetDirectionVector();
    if (std::abs(delta * dir_vector.x) > std::abs(target_x - hitbox_.x)) {
        hitbox_.x = target_x;
    } else {
        hitbox_.x += delta * dir_vector.x;
    }

    if (std::abs(delta * dir_vector.y) > std::abs(target_y - hitbox_.y)) { 
        hitbox_.y = target_y;
    } else {
        hitbox_.y += delta * dir_vector.y;
    }
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

void Ghost::UpdateStateEyes(float dt) {
    if (path_index_ >= path_.size()) {
        SetHousingState();
        return;
    }

    is_moving_between_tiles_ = true;
    const auto [target_row, target_col] = path_[path_index_];
    const auto [target_x, target_y] = game_map_.FromRowColToCoords(
        path_[path_index_].first, path_[path_index_].second);

    if (hitbox_.x < target_x) {
        direction_ = EMovingDirection::RIGHT;
    } else if (hitbox_.x > target_x) {
        direction_ = EMovingDirection::LEFT;
    } else if (hitbox_.y < target_y) {
        direction_ = EMovingDirection::DOWN;
    } else if (hitbox_.y > target_y) {
        direction_ = EMovingDirection::UP;
    } else {
        is_moving_between_tiles_ = false;
        hitbox_.x = target_x;
        hitbox_.y = target_y;
        ++path_index_;
        return;
    }

    const auto delta = static_cast<int>(GhostParameters::kVelocityStateEyes * dt);
    const auto dir_vector = GetDirectionVector();
    if (std::abs(delta * dir_vector.x) > std::abs(target_x - hitbox_.x)) {
        hitbox_.x = target_x;
    } else {
        hitbox_.x += delta * dir_vector.x;
    }

    if (std::abs(delta * dir_vector.y) > std::abs(target_y - hitbox_.y)) { 
        hitbox_.y = target_y;
    } else {
        hitbox_.y += delta * dir_vector.y;
    }
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

    int x = 0;
    int y = 0;
    if (IsInStateChasing() || state_ == EState::HOUSING || state_ == EState::STOP) {
        x = kStartingX +
            ((kPadding + kWidth) * GhostSprite::kAnimationCountChasing) * dir +
            ((kPadding + kWidth) * sprite_index_);
        y = GhostSprite::kStartingY + (kPadding + kHeight) * static_cast<int>(type_);
    } else if (state_ == EState::EYES) {
        x = kStartingX +
            ((kPadding + kWidth) * dir);
        y = 202;
    } else if (state_ == EState::FRIGHTENED) {
        x = kStartingX + ((kPadding + kWidth) * sprite_index_);
        y = 163;
    }
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

void Ghost::Die() {
    // TODO: Spawn score
    state_ = EState::EYES;
    const auto [row, col] = game_map_.FromCoordsToRowCol(hitbox_.x, hitbox_.y);
    const auto [target_row, target_col] = game_map_.FromCoordsToRowCol(starting_position_.x, starting_position_.y);
    path_index_ = 0;
    path_ = pathfinder_.FindPath(row, col, target_row, target_col);
}

void Ghost::SetHousingState() {
    state_ = EState::HOUSING;
    direction_ = starting_direction_;
    timer_mode_house_.Restart();
    timer_mode_house_swap_direction_.Restart();
}

void Ghost::SetStateFrightened() {
    state_ = EState::FRIGHTENED;
}

void Ghost::SetStateStop() {
    state_ = EState::STOP;
}

bool Ghost::IsInStateChasing() const {
    return (state_ == EState::CHASING);
}

bool Ghost::IsInStateFrightened() const {
    return (state_ == EState::FRIGHTENED);
}

bool Ghost::IsInStateEyes() const {
    return (state_ == EState::EYES);
}

const SDL_Rect& Ghost::GetHibox() const {
    return hitbox_;
}