#include "Player.hpp"

#include "Constants.hpp"

#include <stdexcept>
#include <algorithm>

Player::Player(
    Renderer& renderer,
    TextureManager& texture_manager,
    const GameMap& game_map)
    : EntityMovable(renderer, {356, 516, 31, 31}, game_map, PlayerParameters::kVelocity, EDirection::RIGHT)
    , texture_manager_(texture_manager)
    , next_direction_(direction_)
    , state_(EState::READY)
    , lifes_(PlayerParameters::kLifes)
    , score_(0) {
    sprite_sheet_ = texture_manager_.LoadTexture(kAssetsFolderImages + "spritesheet.png");
}

void Player::Reset() {
    state_ = EState::READY;
    direction_ = EDirection::RIGHT;
    dying_animation_timer_.Restart();
    dying_animation_sprite_index_ = 0;
    moving_animation_timer_.Restart();
    moving_animation_sprite_index_ = 0;
    ResetHitBox();
}

void Player::HandleKeyPressed(SDL_Scancode scancode) {
    if (IsDying() || IsDead()) return;

    switch(scancode) {
        case SDL_SCANCODE_UP:
            state_ = EState::MOVING;
            next_direction_ = EDirection::UP;
        break;
        case SDL_SCANCODE_DOWN:
            state_ = EState::MOVING;
            next_direction_ = EDirection::DOWN;
        break;
        case SDL_SCANCODE_RIGHT:
            state_ = EState::MOVING;
            next_direction_ = EDirection::RIGHT;
        break;
        case SDL_SCANCODE_LEFT:
            state_ = EState::MOVING;
            next_direction_ = EDirection::LEFT;
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
        return;
    }

    if (state_ == EState::READY || state_ == EState::MOVING) {
        moving_animation_timer_.Update(dt);
        if (moving_animation_timer_.DidFinish()) {
            moving_animation_sprite_index_ = (moving_animation_sprite_index_ + 1) % PlayerParameters::kMovingAnimationCount;
        }

        if (state_ == EState::MOVING) {
            Step(dt);
        }
    }
}

bool Player::Step(float dt) {
    return (TryToStep(dt, next_direction_) || TryToStep(dt, direction_));
}

void Player::Render() {
    if (state_ == EState::MOVING || state_ == EState::READY) {
        const auto src_r = GetSourceRectMoving();
        const double angle = 90.0 * static_cast<double>(direction_);
        renderer_.RenderTexture(sprite_sheet_, src_r, hitbox_, angle);
    } else if (state_ == EState::DYING) {
        const auto src_r = GetSourceRectDying();
        const double angle = 90.0 * static_cast<double>(direction_);
        renderer_.RenderTexture(sprite_sheet_, src_r, hitbox_, angle);
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

bool Player::IsMoving() const {
    return (state_ == EState::MOVING);
}