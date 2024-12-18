#include "Ghost.hpp"

#include "scenes/GameScene.hpp"

#include "Constants.hpp"

#include <algorithm>
#include <array>
#include <random>

namespace {
static const int kSourceWidth = 14;
static const int kSourceHeight = kSourceWidth;

static const int kSpritesCountMoving = 2;
static const SDL_Rect kSourceAnimationMoving {3, 83, kSourceWidth, kSourceHeight};
static const SDL_Rect kSourceAnimationEyes {3, 202, kSourceWidth, kSourceHeight};
static const SDL_Rect kSourceAnimationFrightened {3, 163, kSourceWidth, kSourceHeight};

static const std::array<SDL_Rect, 4> kSourceAnimationScore {
    SDL_Rect{3, 226, 18, 7},
    SDL_Rect{23, 226, 15, 7},
    SDL_Rect{43, 226, 15, 7},
    SDL_Rect{62, 226, 16, 7}};

static const float kSpeedHousing = 125.f;
static const float kSpeedEyes = 200.f;

static const float kHitboxScale = 0.6f;
}

Ghost::Ghost(
    Renderer& renderer,
    TextureManager& texture_manager,
    const GameMap& game_map,
    Pathfinder& pathfinder,
    const Level& level,
    std::string name,
    EType type,
    SDL_FRect renderer_rect,
    EDirection direction,
    PathfindingPattern pathfinding_pattern)
    : EntityMovable(renderer, renderer_rect, game_map, level.GetSpeedGhost(), direction, .8f)
    , texture_manager_(texture_manager)
    , pathfinder_(pathfinder)
    , level_(level)
    , name_(name)
    , type_(type)
    , patfinder_pattern_(pathfinding_pattern)
    , state_(EState::HOUSING)
    , state_previous_(state_)
    , path_index_(0)
    , is_moving_between_tiles_(false)
    , animation_timer_(0.1f)
    , sprite_index_(0) {
    Init();
}

void Ghost::Init() {
    sprite_sheet_ = texture_manager_.LoadTexture(kAssetsFolderImages + "spritesheet.png");
    SetStateStop();
    
    animation_timer_.SetOnFinishCallback([this]() {
        sprite_index_ = (sprite_index_ + 1) % kSpritesCountMoving;
    });

    timer_mode_house_.SetOnFinishCallback([this]() { SetStateChasing(); });
    timer_mode_house_swap_direction_.SetOnFinishCallback([this]() { ReverseDirection(); });

    timer_frightened_intermittent_.SetOnFinishCallback([this]() {
        frightened_animation_index_ = !frightened_animation_index_;
    });

    timer_showing_score_.SetOnFinishCallback([this]() { SetState(EState::EYES); });
}

void Ghost::Reset() {
    Entity::Reset();
    direction_ = starting_direction_;
    SetStateStop();
    path_.clear();
}

void Ghost::Update(float dt, GameScene* game_scene) {
    if (state_ == EState::STOP) return;

    animation_timer_.Update(dt);
    switch(state_) {
        case EState::HOUSING:       UpdateStateHouse(dt);                   break;
        case EState::FRIGHTENED:    UpdateStateFrightened(dt, *game_scene); break;
        case EState::CHASING:       UpdateStateChasing(dt, *game_scene);    break;
        case EState::SHOWING_SCORE: timer_showing_score_.Update(dt);        break;
        case EState::EYES:          UpdateStateEyes(dt, *game_scene);                    break;
    }
}

void Ghost::UpdateStateHouse(float dt) {
    timer_mode_house_.Update(dt);
    timer_mode_house_swap_direction_.Update(dt);
    Step(dt);
}

void Ghost::UpdateStateChasing(float dt, GameScene& game_scene) {
    if (!is_moving_between_tiles_) {
        FindPath(game_scene);
    }

    if (path_index_ >= path_.size()) {
        CenterAxis();
        return;
    }

    StepPath(dt);
}

// Should avoid finding path when is moving between tiles.
void Ghost::FindPath(GameScene& game) {
    if (is_moving_between_tiles_) return;

    const auto& center_pos = GetCenterPosition();
    const auto col_row = game_map_.FromCoordsToColRow(center_pos);
    path_index_ = 1;
    path_ = patfinder_pattern_(col_row, game);
}

void Ghost::StepPath(float dt) {
    is_moving_between_tiles_ = true;

    const auto& target_cell = game_map_.GetCell(path_[path_index_]); 
    const auto target_coords = target_cell.center;

    SetDirectionByTarget(target_coords);
    Step(dt);

    constexpr float threshold = 4.f;
    auto diff_length = (GetCenterPosition() - target_coords).Length();
    if (diff_length <= threshold) {
        is_moving_between_tiles_ = false;
        path_index_++;
    }
}

void Ghost::UpdateStateFrightened(float dt, GameScene& game_scene) {
    if (game_scene.GetSecondsToFinishFrightenedMode() <= intermittent_time_last_seconds_) {
        timer_frightened_intermittent_.Update(dt);
    }

    const auto& cell = game_map_.GetCell(GetCenterPosition());
    if (DidReachCellCenter() && cell.cell_index != last_visited_cell_index_) {
        const auto dir = ChooseRandomDirection();
        if (IsOrthogonalTurn(dir)) {
            direction_ = dir;
        }
        last_visited_cell_index_ = cell.cell_index;
    }

    Step(dt);
}

EDirection Ghost::ChooseRandomDirection() const {
    std::array directions {
        EDirection::LEFT,
        EDirection::UP,
        EDirection::DOWN,
        EDirection::RIGHT
    };

    auto is_unwanted_direction = [&](EDirection d) {
        return (!IsMovableDirection(d) || d == GetOppositeDirection());
    };
    auto directions_end = std::remove_if(directions.begin(), directions.end(), is_unwanted_direction);
    if (std::distance(directions.begin(), directions_end) > 1) {
        static thread_local std::mt19937 rng{std::random_device{}()};
        std::ranges::shuffle(directions.begin(), directions_end, rng);
    }
    
    return *directions.begin();
}

void Ghost::UpdateStateEyes(float dt, GameScene& game_scene) {
    if (path_index_ >= path_.size()) {
        SetStateHousing();
        game_scene.GhostInEyesStateArrivedToHouse();    
        return;
    }

    StepPath(dt);
    const auto dir_vector = GetDirectionVector();
    if (dir_vector.y != 0) {
        CenterAxisX();
    } else if (dir_vector.x != 0) {
        CenterAxisY();
    }
}

void Ghost::Render() {
    SDL_Rect src_r;
    SDL_FRect dst_r = GetRendererRect();
    switch(state_) {
        case EState::STOP:
        case EState::HOUSING:
        case EState::CHASING:
            src_r = kSourceAnimationMoving;
            src_r.x += (6 + src_r.w) * kSpritesCountMoving * GetSpriteIndexByDirection() +
                       (6 + src_r.w) * sprite_index_;
            src_r.y += (6 + src_r.h) * static_cast<int>(type_);
        break;
        case EState::SHOWING_SCORE: {
            src_r = kSourceAnimationScore[showing_score_index_];
            dst_r.w = 32.f;
            dst_r.h = 14.f;
            const auto center = GetCenterPosition();
            dst_r.x = center.x - dst_r.w / 2.f;
            dst_r.y = center.y - dst_r.h / 2.f;
        break;
        }
        case EState::EYES:
            src_r = kSourceAnimationEyes;
            src_r.x += (6 + src_r.w) * GetSpriteIndexByDirection();
        break;
        case EState::FRIGHTENED:
            src_r = kSourceAnimationFrightened;
            src_r.x += (6 + src_r.w) * sprite_index_ + (6 + src_r.w) * frightened_animation_index_;
        break;
    }

    renderer_.RenderTexture(sprite_sheet_, src_r, dst_r);
}

int Ghost::GetSpriteIndexByDirection() const {
    switch(direction_) {
        default:
        case EDirection::UP:    return 0;
        case EDirection::DOWN:  return 1;
        case EDirection::LEFT:  return 2;
        case EDirection::RIGHT: return 3;
    }
}

const std::string_view Ghost::GetName() const {
    return name_;
}

unsigned int Ghost::Die(int died_in_same_frightened_count) {
    showing_score_index_ = died_in_same_frightened_count - 1;
    SetState(EState::SHOWING_SCORE);
    velocity_ = kSpeedEyes;
    const auto& hitbox = GetHitBox();
    const auto col_row_from = game_map_.FromCoordsToColRow({hitbox.x, hitbox.y});
    const auto fixed_coords = game_map_.FromColRowToCoords(col_row_from);
    UpdatePosition(fixed_coords);
    
    path_index_ = 0;
    const auto col_row_to = game_map_.FromCoordsToColRow(
        Vec2<float>{starting_renderer_rect_.x, starting_renderer_rect_.y});
    path_ = pathfinder_.FindPath(col_row_from, col_row_to);
    
    return (kScoreBase * died_in_same_frightened_count);
}

void Ghost::SetStateChasing() {
    SetState(EState::CHASING);
    velocity_ = level_.GetSpeedGhost();
}

void Ghost::SetStateHousing() {
    SetState(EState::HOUSING);
    velocity_ = kSpeedHousing;
    direction_ = starting_direction_;
    timer_mode_house_.Restart();
    timer_mode_house_swap_direction_.Restart();
    is_moving_between_tiles_ = false;
}

void Ghost::SetStateFrightened() {
    SetState(EState::FRIGHTENED);
    velocity_ = level_.GetSpeedGhostFrightened();
    timer_frightened_intermittent_.Restart();
    frightened_animation_index_ = 0;
    is_moving_between_tiles_ = false;
    path_.clear();
}

void Ghost::SetState(EState new_state) {
    state_previous_ = state_;
    state_ = new_state;
}

void Ghost::SetStateEyes() {
    SetState(EState::EYES);
}

void Ghost::SetStateStop() {
    SetState(EState::STOP);
}

bool Ghost::IsInStateChasing() const {
    return (state_ == EState::CHASING);
}

bool Ghost::IsInStateShowingScore() const {
    return (state_ == EState::SHOWING_SCORE);
}

bool Ghost::IsInStateFrightened() const {
    return (state_ == EState::FRIGHTENED);
}

bool Ghost::IsInStateEyes() const {
    return (state_ == EState::EYES);
}
