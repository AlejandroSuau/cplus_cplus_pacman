#include "Ghost.hpp"

#include "Constants.hpp"
#include "Game.hpp"

#include <algorithm>
#include <array>
#include <random>

Ghost::Ghost(
    Renderer& renderer,
    TextureManager& texture_manager,
    const GameMap& game_map,
    Pathfinder& pathfinder,
    std::string name,
    EType type,
    SDL_FRect renderer_rect,
    float velocity,
    EDirection direction,
    PathfindingPattern pathfinding_pattern)
    : EntityMovable(renderer, renderer_rect, game_map, 100.f, direction, .6f)
    , texture_manager_(texture_manager)
    , pathfinder_(pathfinder)
    , name_(name)
    , type_(type)
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
    Entity::Reset();
    direction_ = starting_direction_;
    state_ = EState::HOUSING;
    is_moving_between_tiles_ = false;
    timer_mode_house_.Restart();
    timer_mode_house_swap_direction_.Restart();
    path_.clear();
}

// Should avoid finding path when is moving between tiles.
void Ghost::FindPath(Game& game) {
    if (is_moving_between_tiles_) return;

    const auto& center_pos = GetCenterPosition();
    const auto col_row = game_map_.FromCoordsToColRow(center_pos);
    path_index_ = 1;
    path_ = patfinder_pattern_(col_row, game);
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
    if (timer_mode_house_swap_direction_.DidFinish()) {
        ReverseDirection();
    }

    Step(dt);
    if (timer_mode_house_.DidFinish()) {
        state_ = EState::CHASING;
    }
}

void Ghost::UpdateStateChasing(float dt) {
    if (path_index_ >= path_.size()) return;

    StepPath(dt);
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

void Ghost::UpdateStateFrightened(float dt) {
    timer_mode_frightened_.Update(dt);
    if (timer_mode_frightened_.GetSecondsToFinish() <= intermittent_time_last_seconds_) {
        timer_frightened_intermittent_.Update(dt);
        if (timer_frightened_intermittent_.DidFinish()) {
            frightened_animation_index = !frightened_animation_index;
        }
    }

    if (timer_mode_frightened_.DidFinish()) {
        state_ = EState::CHASING;
        return;
    }

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

    const auto& cell = game_map_.GetCell(GetCenterPosition());
    if (DidReachCellCenter() && cell.cell_index != last_visited_cell_index_) {
        const auto dir = *directions.begin();
        if (IsOrthogonalTurn(dir)) {
            direction_ = dir;
        }
        last_visited_cell_index_ = cell.cell_index;
    }

    Step(dt);
}

void Ghost::UpdateStateEyes(float dt) {
    if (path_index_ >= path_.size()) {
        SetHousingState();
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
    RenderPath();
    const auto src_r = GetSourceRect();
    renderer_.SetRenderingColor({0, 100, 150, 255});
    renderer_.RenderRectFilled(GetHitBox());
    // renderer_.RenderRectFilled(GetRendererRect());
    // renderer_.RenderTexture(sprite_sheet_, src_r, GetRendererRect());
}

void Ghost::RenderPath() {
    renderer_.SetRenderingColor({200, 200, 200, 50});
    const auto cell_size = game_map_.GetCellSizeFloat();
    std::size_t i = 0;
    for (const auto& col_row : path_) {
        const auto& cell = game_map_.GetCell(col_row);
        SDL_FRect r {cell.position.x, cell.position.y, cell_size, cell_size};
        renderer_.RenderRectFilled(r);
        if (i == path_index_) {
            renderer_.SetRenderingColor({255, 50, 50, 50});
            renderer_.RenderRectFilled(r);
        }
        ++i;
    }
}

SDL_Rect Ghost::GetSourceRect() const {
    using namespace SpriteSheet;
    int asset_by_direction;
    switch(direction_) {
        case EDirection::UP:    asset_by_direction = 0; break; 
        case EDirection::DOWN:  asset_by_direction = 1; break;
        case EDirection::LEFT:  asset_by_direction = 2; break;
        case EDirection::RIGHT: asset_by_direction = 3; break;
    }

    int x = 0;
    int y = 0;
    if (IsInStateChasing() || state_ == EState::HOUSING || state_ == EState::STOP) {
        x = kStartingX +
            ((kPadding + kWidth) * GhostSprite::kAnimationCountChasing) * asset_by_direction +
            ((kPadding + kWidth) * sprite_index_);
        y = GhostSprite::kStartingY + (kPadding + kHeight) * static_cast<int>(type_);
    } else if (state_ == EState::EYES) {
        x = kStartingX +
            ((kPadding + kWidth) * asset_by_direction);
        y = 202;
    } else if (state_ == EState::FRIGHTENED) {
        x = kStartingX + ((kPadding + kWidth) * sprite_index_) +
            (kPadding + kWidth) * (frightened_animation_index);
        y = 163;
    }
    return SDL_Rect {x, y, kWidth, kHeight};
}

const std::string_view Ghost::GetName() const {
    return name_;
}

void Ghost::Die() {
    // TODO: Spawn score
    state_ = EState::EYES;
    const auto& hitbox = GetHitBox();
    const auto col_row_from = game_map_.FromCoordsToColRow({hitbox.x, hitbox.y});
    const auto fixed_coords = game_map_.FromColRowToCoords(col_row_from);
    UpdatePosition(fixed_coords);
    
    path_index_ = 0;
    const auto col_row_to = game_map_.FromCoordsToColRow(
        Vec2<float>{starting_renderer_rect_.x, starting_renderer_rect_.y});
    path_ = pathfinder_.FindPath(col_row_from, col_row_to);
}

void Ghost::SetHousingState() {
    state_ = EState::HOUSING;
    direction_ = starting_direction_;
    timer_mode_house_.Restart();
    timer_mode_house_swap_direction_.Restart();
}

void Ghost::SetStateFrightened() {
    state_ = EState::FRIGHTENED;
    timer_mode_frightened_.Restart();
    timer_frightened_intermittent_.Restart();
    frightened_animation_index = 0;
    is_moving_between_tiles_ = false;
    path_.clear();
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
