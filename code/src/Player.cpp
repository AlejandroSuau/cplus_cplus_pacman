#include "Player.hpp"

#include "Constants.hpp"

#include <stdexcept>
#include <algorithm>

namespace {
    static const SDL_FRect kRendererRect {356.f, 516.f, 30.f, 32.f};
}

Player::Player(
    Renderer& renderer,
    TextureManager& texture_manager,
    const GameMap& game_map)
    : EntityMovable(renderer, {356.f, 516.f, 24.f, 24.f}, game_map, 125.f, EDirection::RIGHT, .5f)
    , texture_manager_(texture_manager)
    , next_direction_(direction_)
    , state_(EState::READY)
    , lifes_(PlayerParameters::kLifes)
    , score_(0) {
    sprite_sheet_ = texture_manager_.LoadTexture(kAssetsFolderImages + "spritesheet.png");
}

void Player::Reset() {
    ResetHitBox();
    state_ = EState::READY;
    direction_ = EDirection::RIGHT;
    dying_animation_timer_.Restart();
    dying_animation_sprite_index_ = 0;
    moving_animation_timer_.Restart();
    moving_animation_sprite_index_ = 0;
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
    auto did_reach_cell_center = [&]() {
        const auto center_rect = GetCenterPosition();
        const auto center_cell = game_map_.FromCoordsToCenterCellCoords(center_rect);
        const auto tolerance = 4.f;
        return (fabs(center_rect.x - center_cell.x) <= tolerance &&
                fabs(center_rect.y - center_cell.y) <= tolerance);
    };
    
    auto is_valid_direction = [&](EDirection dir) -> bool {
        const auto center_rect = GetCenterPosition();
        const auto center_cell = game_map_.FromCoordsToCenterCellCoords(center_rect);
        auto col_row = game_map_.FromCoordsToColRow(center_rect);
        const auto dir_vector = GetDirectionVector(dir);
        
        bool has_passed_center = false;
        const auto tolerance = 4.f;
        if (dir_vector.x != 0 && fabs(center_rect.x - center_cell.x) < tolerance) {
            has_passed_center = true;
        } else if (dir_vector.y != 0 && fabs(center_rect.y - center_cell.y) < tolerance) {
            has_passed_center = true;
        }

        if (has_passed_center) {
            col_row += dir_vector;
        }

        return game_map_.AreColRowWalkable(col_row);
    };

    // CHANGE DIR ONLY ON MIDDLE OF CELL
    if (did_reach_cell_center() && is_valid_direction(next_direction_)) {
        direction_ = next_direction_;
    }

    bool r = false;
    if (is_valid_direction(direction_)) {
        r = EntityMovable::Step(dt);
    }

    // CENTER IT
    auto rect = GetRendererRect();
    Vec2 pos_rect {rect.x, rect.y};
    auto cell = game_map_.GetCell(pos_rect);
    auto cell_size = game_map_.GetCellSizeFloat();
    if (direction_ == EDirection::DOWN || direction_ == EDirection::UP) {
        pos_rect.x = cell.position.x + 4.f;//(cell_size - rect.w);
    } else if (direction_ == EDirection::LEFT || direction_ == EDirection::RIGHT) {
        pos_rect.y = cell.position.y + 4.f;//(cell_size - rect.h);
    }

    UpdatePosition(pos_rect);
    return r;
}

void Player::Render() {
    renderer_.SetRenderingColor({255, 0, 0, 255});
    renderer_.RenderRect(GetHitBox());

    if (state_ == EState::MOVING || state_ == EState::READY) {
        const auto src_r = GetSourceRectMoving();
        const double angle = 90.0 * static_cast<double>(direction_);
        renderer_.RenderTexture(sprite_sheet_, src_r, GetRendererRect(), angle);
    } else if (state_ == EState::DYING) {
        const auto src_r = GetSourceRectDying();
        const double angle = 90.0 * static_cast<double>(direction_);
        renderer_.RenderTexture(sprite_sheet_, src_r, GetRendererRect(), angle);
    }
}

SDL_Rect Player::GetSourceRectMoving() const {
    using namespace SpriteSheet;
    //const int x = kStartingX + (kPadding + kWidth) * moving_animation_sprite_index_;
    SDL_Rect src_rect {282, 82, 32, 32};
    src_rect.y = 2 + (8 + 32) * moving_animation_sprite_index_;
    return src_rect;
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