#include "Player.hpp"

#include "Constants.hpp"

#include <stdexcept>
#include <algorithm>

namespace {
static const unsigned int kMaxLifes = 6;
static const unsigned int kStartingLifes = 3;

static const float kStartingX = 356.f;
static const float kStartingY = 516.f;
static const float kWidth = 26.f;
static const float kHeight = kWidth;

static const float kHitboxScale = 0.6f;

static const int kSpritesCountMoving {3};
static const SDL_Rect kSourceAnimationMoving {282, 2, 32, 32}; // next y: (8 + width) * sprite_index_moving_

static const int kSpritesCountDying {11};
static const SDL_Rect kSourceAnimationDying {3, 245, 14, 14}; // next x: (6 + width) * sprite_index_dying_ 
}

Player::Player(
    Renderer& renderer,
    TextureManager& texture_manager,
    const GameMap& game_map,
    const Level& level)
    : EntityMovable(
        renderer,
        {kStartingX, kStartingY, kWidth, kHeight},
        game_map,
        level.GetSpeedPlayer(),
        EDirection::RIGHT,
        kHitboxScale)
    , texture_manager_(texture_manager)
    , level_(level)
    , next_direction_(direction_)
    , state_(EState::STOP)
    , lifes_(kStartingLifes)
    , score_(0) {
    Init();
}

void Player::Init() {
    sprite_sheet_ = texture_manager_.LoadTexture(kAssetsFolderImages + "spritesheet.png");
    CenterAxis();

    animation_timer_dying_.SetOnFinishCallback([this]() {
        state_ = (++sprite_index_dying_ == kSpritesCountDying) ? EState::DEAD : state_;
    });

    animation_timer_moving_.SetOnFinishCallback([this]() {
        sprite_index_moving_ =  (sprite_index_moving_ + 1) % kSpritesCountMoving;
    });
}

void Player::Reset() {
    Entity::Reset();
    state_ = EState::STOP;
    direction_ = EDirection::RIGHT;
    velocity_ = level_.GetSpeedPlayer();
    CenterAxis();
    
    animation_timer_dying_.Restart();
    sprite_index_dying_ = 0;
    animation_timer_moving_.Restart();
    sprite_index_moving_ = 2;
}

void Player::HandleKeyPressed(const SDL_Scancode& scancode) {
    if (state_ != EState::READY && state_ != EState::MOVING) return;
    
    state_ = EState::MOVING;
    switch(scancode) {
        case SDL_SCANCODE_UP:    next_direction_ = EDirection::UP;    break;
        case SDL_SCANCODE_DOWN:  next_direction_ = EDirection::DOWN;  break;
        case SDL_SCANCODE_RIGHT: next_direction_ = EDirection::RIGHT; break;
        case SDL_SCANCODE_LEFT:  next_direction_ = EDirection::LEFT;  break;
    }
}

void Player::Update(float dt, GameScene* /*game*/) {
    switch(state_) {
        case EState::MOVING: UpdateStateMoving(dt);             break;
        case EState::DYING:  animation_timer_dying_.Update(dt); break;
    }
}

void Player::UpdateStateMoving(float dt) {   
    bool should_move = false;
    if (IsMovableDirection(next_direction_)) {
        should_move = IsOrthogonalTurn(next_direction_) ? DidReachCellCenter() : true;
        if (should_move) direction_ = next_direction_;
    }
    
    if (should_move || IsMovableDirection(direction_)) {
        animation_timer_moving_.Update(dt);
        Step(dt);        
    }
}

void Player::Render() {
    if (IsDead()) return;

    SDL_Rect src_r;
    double angle;
    if (IsDying()) {
        src_r = kSourceAnimationDying;
        src_r.x += (6 + src_r.w) * sprite_index_dying_;
        angle = GetAssetAngleOnDying();
    } else {
        src_r = kSourceAnimationMoving;
        src_r.y += (8 + src_r.h) * sprite_index_moving_;
        angle = GetAssetAngleOnMoving();
    }

    renderer_.RenderTexture(sprite_sheet_, src_r, GetRendererRect(), angle);
}

double Player::GetAssetAngleOnDying() const {
    switch(direction_) {
        case EDirection::UP:    return 0;
        case EDirection::RIGHT: return 90;
        case EDirection::DOWN:  return 180;
        case EDirection::LEFT:  return 270;
    }
}

double Player::GetAssetAngleOnMoving() const {
    return (90.0 * static_cast<double>(direction_));
}

void Player::Stop() {
    state_ = EState::STOP;
}

void Player::Die() {
    state_ = EState::DYING;
    DecreaseOneLife();
}

void Player::SetStateReady() {
    state_ = EState::READY;
}

void Player::IncreaseOneLife() {
    lifes_ = std::clamp(lifes_, lifes_ + 1, kMaxLifes);
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

bool Player::HasLifes() const {
    return lifes_ > 0;
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
