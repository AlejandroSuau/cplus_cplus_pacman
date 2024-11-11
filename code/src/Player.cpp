#include "Player.hpp"

#include "Constants.hpp"

Player::Player(
    TextureManager& texture_manager,
    GameMap& game_map,
    Pathfinder& pathfinder,
    ScoreManager& score_manager,
    Vec2 position)
    : texture_manager_(texture_manager)
    , game_map_(game_map)
    , pathfinder_(pathfinder)
    , score_manager_(score_manager)
    , position_(position)
    , color_{0, 255, 0, 255}
    , moving_timer_(300)
    , moving_direction_(EMovingDirection::LEFT) {}

void Player::Update(float dt) {
    if (moving_timer_.HasElapsed()) {
        Move();
    }
}

void Player::HandleKeyPressed(SDL_Scancode scancode) {
    switch(scancode) {
        case SDL_SCANCODE_UP:
            moving_direction_ = EMovingDirection::UP;
        break;
        case SDL_SCANCODE_DOWN:
            moving_direction_ = EMovingDirection::DOWN;
        break;
        case SDL_SCANCODE_RIGHT:
            moving_direction_ = EMovingDirection::RIGHT;
        break;
        case SDL_SCANCODE_LEFT:
            moving_direction_ = EMovingDirection::LEFT;
        break;
    }
}

void Player::Move() {
    const auto prev_position = position_;
    const auto distance = static_cast<float>(kCellSize);
    switch(moving_direction_) {
        case EMovingDirection::DOWN:
            position_.y += distance;
        break;
        case EMovingDirection::UP:
            position_.y -= distance;
        break;
        case EMovingDirection::LEFT:
            position_.x -= distance;
        break;
        case EMovingDirection::RIGHT:
            position_.x += distance;
        break;
    }

    if (!game_map_.AreCoordsWalkable(position_.x, position_.y)) {
        position_ = prev_position;
    }
}

void Player::ProcessGhostCollision() {
    // Depends on player status
        // Die?
        // Destroy
    int x = 10;
    score_manager_.IncreaseScore(x);
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
    SDL_FRect rect {
        position_.x,
        position_.y,
        static_cast<int>(kCellSize),
        static_cast<int>(kCellSize)};
    
    SDL_SetRenderDrawColor(&renderer, color_.r, color_.g, color_.b, color_.a);
    SDL_RenderFillRectF(&renderer, &rect);
}

const Vec2& Player::GetPosition() const {
    return position_;
}