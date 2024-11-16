#include "Player.hpp"

#include "Constants.hpp"

#include <stdexcept>
#include <algorithm>

Player::Player(
    TextureManager& texture_manager,
    GameMap& game_map,
    Pathfinder& pathfinder,
    ScoreManager& score_manager)
    : texture_manager_(texture_manager)
    , game_map_(game_map)
    , pathfinder_(pathfinder)
    , score_manager_(score_manager)
    , hitbox_{0, 0, PlayerParameters::kWidth, PlayerParameters::kHeight}
    , direction_(EMovingDirection::RIGHT)
    , next_direction_(direction_)
    , status_(EStatus::READY)
    , lifes_(PlayerParameters::kLifes) {
    sprite_sheet_ = texture_manager_.LoadTexture(kAssetsFolderImages + "spritesheet.png");
    const auto starting_coords = game_map_.FromRowColToCoords(13, 8);
    hitbox_.x = starting_coords.first;
    hitbox_.y = starting_coords.second;
}

void Player::HandleKeyPressed(SDL_Scancode scancode) {
    switch(scancode) {
        case SDL_SCANCODE_UP:
            status_ = EStatus::MOVING;
            next_direction_ = EMovingDirection::UP;
        break;
        case SDL_SCANCODE_DOWN:
            status_ = EStatus::MOVING;
            next_direction_ = EMovingDirection::DOWN;
        break;
        case SDL_SCANCODE_RIGHT:
            status_ = EStatus::MOVING;
            next_direction_ = EMovingDirection::RIGHT;
        break;
        case SDL_SCANCODE_LEFT:
            status_ = EStatus::MOVING;
            next_direction_ = EMovingDirection::LEFT;
        break;
    }
}

void Player::Update(float dt) {
    if (status_ == EStatus::DYING) {
        dying_animation_timer_.Update(dt);
        if (dying_animation_timer_.DidFinish()) {
            ++dying_animation_sprite_index_;
            if (dying_animation_sprite_index_ == PlayerParameters::kDyingAnimationCount) {
                status_ = EStatus::DEAD;
            }
        }
    }

    if (status_ == EStatus::MOVING) {
        moving_animation_timer_.Update(dt);
        if (moving_animation_timer_.DidFinish()) {
            moving_animation_sprite_index_ = (moving_animation_sprite_index_ + 1) % PlayerParameters::kMovingAnimationCount;
        }
        Move(dt);
    }
}

void Player::Move(float dt) {
    const auto dx = static_cast<int>(PlayerParameters::kVelocity * dt);
    const auto dy = static_cast<int>(PlayerParameters::kVelocity * dt);
    if (!TryToMove(next_direction_, dx, dy)) {
        TryToMove(direction_, dx, dy);
    }
}

bool Player::TryToMove(EMovingDirection direction, int dx, int dy) {
    const auto prev_x = hitbox_.x;
    const auto prev_y = hitbox_.y;
    switch(direction) {
        case EMovingDirection::DOWN:  hitbox_.y += dy; break;
        case EMovingDirection::UP:    hitbox_.y -= dy; break;
        case EMovingDirection::LEFT:  hitbox_.x -= dx; break; 
        case EMovingDirection::RIGHT: hitbox_.x += dx; break;
    }
    
    if (!IsMovementAllowed(hitbox_)) {
        hitbox_.x = prev_x;
        hitbox_.y = prev_y;
        return false;
    }

    direction_ = direction;
    return true;
}

bool Player::IsMovementAllowed(SDL_Rect moved_rect) const {
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

void Player::ProcessGhostCollision() {
    status_ = EStatus::DYING;
    // Depends on player status
        // Die?
        // Destroy
    //int x = 10;
    //score_manager_.IncreaseScore(x);
}

void Player::ProcessBigCoinCollision() {
    // Increase Score
    // Change ghost status
    int x = 10;
    score_manager_.IncreaseScore(x);
}

void Player::ProcessCoinCollision() {
    // Increase Score.
    int x = 10;
    score_manager_.IncreaseScore(x);
}

void Player::Render(SDL_Renderer& renderer) {
    auto src_r = GetSourceRectMoving();
    const double angle = 90.0 * static_cast<double>(direction_);
    SDL_RenderCopyEx(&renderer, sprite_sheet_, &src_r, &hitbox_, angle, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
    
    /*auto src_r = GetSourceRectDying();
    // const double angle = 90.0 * static_cast<double>(direction_);
    SDL_RenderCopyEx(&renderer, sprite_sheet_, &src_r, &dst_r, 0, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);*/
}

SDL_Rect Player::GetSourceRectMoving() const {
    using namespace SpriteSheet;
    const int x = kStartingX + (kPadding + kWidth) * moving_animation_sprite_index_;
    return {x, kStartingY, kWidth, kHeight};
}

SDL_Rect Player::GetSourceRectDying() const {
    using namespace SpriteSheet;
    const int x = kStartingX + (kPadding + kWidth) * dying_animation_sprite_index_;
    return {x, 245, kWidth, kHeight};
}

const SDL_Rect& Player::GetHitbox() const {
    return hitbox_;
}

void Player::DecreaseOneLife() {
    if (lifes_ == 0) {
        throw std::runtime_error("Trying to decrease lifes on already died player!");
        return;
    }
    --lifes_;
}

unsigned int Player::GetLifes() const {
    return lifes_;
}