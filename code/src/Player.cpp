#include "Player.hpp"

#include "Constants.hpp"

#include <stdexcept>
#include <algorithm>

Player::Player(
    TextureManager& texture_manager,
    const GameMap& game_map,
    Pathfinder& pathfinder)
    : texture_manager_(texture_manager)
    , game_map_(game_map)
    , pathfinder_(pathfinder)
    , hitbox_{356, 516, PlayerParameters::kWidth, PlayerParameters::kHeight}
    , starting_position_(hitbox_.x, hitbox_.y)
    , direction_(Player::EMovingDirection::RIGHT)
    , next_direction_(direction_)
    , state_(EState::READY)
    , lifes_(PlayerParameters::kLifes)
    , score_(0) {
    sprite_sheet_ = texture_manager_.LoadTexture(kAssetsFolderImages + "spritesheet.png");
}

void Player::Reset() {
    score_ = 0;
    state_ = EState::READY;
    hitbox_.x = starting_position_.x;
    hitbox_.y = starting_position_.y;
    dying_animation_timer_.Restart();
    dying_animation_sprite_index_ = 0;
    moving_animation_timer_.Restart();
    moving_animation_sprite_index_ = 0;
}

void Player::HandleKeyPressed(SDL_Scancode scancode) {
    switch(scancode) {
        case SDL_SCANCODE_UP:
            state_ = EState::MOVING;
            next_direction_ = EMovingDirection::UP;
        break;
        case SDL_SCANCODE_DOWN:
            state_ = EState::MOVING;
            next_direction_ = EMovingDirection::DOWN;
        break;
        case SDL_SCANCODE_RIGHT:
            state_ = EState::MOVING;
            next_direction_ = EMovingDirection::RIGHT;
        break;
        case SDL_SCANCODE_LEFT:
            state_ = EState::MOVING;
            next_direction_ = EMovingDirection::LEFT;
        break;
    }
}

void Player::Update(float dt) {
    if (state_ == EState::DEAD) return;

    if (state_ == EState::DYING) {
        dying_animation_timer_.Update(dt);
        if (dying_animation_timer_.DidFinish()) {
            ++dying_animation_sprite_index_;
            if (dying_animation_sprite_index_ == PlayerParameters::kDyingAnimationCount) {
                state_ = EState::DEAD;
            }
        }
    }

    if (state_ == EState::READY || state_ == EState::MOVING) {
        moving_animation_timer_.Update(dt);
        if (moving_animation_timer_.DidFinish()) {
            moving_animation_sprite_index_ = (moving_animation_sprite_index_ + 1) % PlayerParameters::kMovingAnimationCount;
        }

        if (state_ == EState::MOVING) {
            Move(dt);
        }
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

void Player::Render(SDL_Renderer& renderer) {
    if (state_ == EState::MOVING || state_ == EState::READY) {
        auto src_r = GetSourceRectMoving();
        const double angle = 90.0 * static_cast<double>(direction_);
        SDL_RenderCopyEx(&renderer, sprite_sheet_, &src_r, &hitbox_, angle, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
    } else if (state_ == EState::DYING) {
        auto src_r = GetSourceRectDying();
        const double angle = 90.0 * static_cast<double>(direction_);
        SDL_RenderCopyEx(&renderer, sprite_sheet_, &src_r, &hitbox_, angle, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
    }
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

Vec2 Player::GetPosition() const {
    return {hitbox_.x, hitbox_.y};
}

const SDL_Rect& Player::GetHitbox() const {
    return hitbox_;
}

void Player::Die() {
    state_ = EState::DYING;
    DecreaseOneLife();
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

Player::EMovingDirection Player::GetDirection() const {
    return direction_;
}

Vec2 Player::GetDirectionVector() const {
    switch(direction_) {
        default:
        case EMovingDirection::DOWN:  return { 0,  1};
        case EMovingDirection::UP:    return { 0, -1};
        case EMovingDirection::LEFT:  return {-1,  0};
        case EMovingDirection::RIGHT: return { 1,  0};
    }
}

void Player::IncreaseScore(unsigned int value) {
    score_ += value;
}

unsigned int Player::GetScore() const {
    return score_;
}

bool Player::IsDying() const {
    return (state_ == EState::DYING);
}

bool Player::IsDead() const {
    return (state_ == EState::DEAD);
}