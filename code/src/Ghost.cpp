#include "Ghost.hpp"

#include "Constants.hpp"
#include "Game.hpp"

// Inky Azul
// Target: 
//          1- 2 casillas en direcc del player (punto imaginario)
//              Coge a blinky y trza una línea entre el y ese punto.
//              Distancia entre blinky y el punto. Y se la suma al punto.

// Pinky (Rosa)
//      4 tiles delante de la dirección de Pacman y 4 a la izquierda (debido a un error en la lógica original).
    // ghost
    // player
    // mapa

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
    , position_(x, y)
    , direction_(direction)
    , patfinder_pattern_(pathfinding_pattern)
    , mode_(EMode::HOUSE)
    , path_index_(0)
    , path_step_timer_(0.5f)
    , animation_timer_(0.1f)
    , sprite_index_(0)
    , sprites_count_(2) {
    
    sprite_sheet_ = texture_manager_.LoadTexture(kAssetsFolderImages + "spritesheet.png");
}

void Ghost::FindPath(Game& game) {
    const auto [row, col] = game_map_.FromCoordsToRowCol(position_.x, position_.y);
    path_step_timer_.Restart();
    path_index_ = 0;
    path_ = patfinder_pattern_({col, row}, game);
}

void Ghost::Update(float dt) {
    animation_timer_.Update(dt);
    if (animation_timer_.DidFinish()) {
        sprite_index_ = (sprite_index_ + 1) % sprites_count_;
    }

    if (mode_ == EMode::HOUSE) {
        timer_mode_house_.Update(dt);
        timer_mode_house_swap_direction_.Update(dt);
        const auto direction_vector = GetDirectionVector();
        const auto dx = static_cast<int>(GhostParameters::kHouseModeVelocity * dt) * direction_vector.x;
        const auto dy = static_cast<int>(GhostParameters::kHouseModeVelocity * dt) * direction_vector.y;
        position_.x += dx;
        position_.y += dy;
        if (timer_mode_house_swap_direction_.DidFinish()) {
            SwapToOpositeDirection();
        }
        if (timer_mode_house_.DidFinish()) {
            mode_ = EMode::CHASING;
        }
    }

    if (mode_ == EMode::CHASING) {
        path_step_timer_.Update(dt);
        if (!path_.empty() && path_step_timer_.DidFinish()) {
            ++path_index_;
            if (path_index_ >= path_.size()) return;

            const auto prev_pos = position_;
            const auto row_col = path_[path_index_];
            const auto new_coords = game_map_.FromRowColToCoords(row_col.first, row_col.second);
            position_.x = new_coords.first;
            position_.y = new_coords.second;
            if (position_.y < prev_pos.y) {
                direction_ = EMovingDirection::UP;
            } else if (position_.y > prev_pos.y){
                direction_ = EMovingDirection::DOWN;
            } else if (position_.x > prev_pos.x) {
                direction_ = EMovingDirection::RIGHT;
            } else if (position_.x < prev_pos.x) {
                direction_ = EMovingDirection::LEFT;
            }
        }
    }
}

void Ghost::ActivateFrightenedMode() {
    mode_ = EMode::FRIGHTENED;
    // render el otro
    // comenzar el tempor
    // Si han pasado X segundos, cambiamos la imagen durante 1s. luego vuelve y luego vuelve a hacerlo
    // despues termina vulnerability

    // igual pero 163
}

void Ghost::Render(SDL_Renderer& renderer) {
    RenderPath(renderer);

    auto src_r = GetSourceRect();
    SDL_Rect dst_r {
        static_cast<int>(position_.x),
        static_cast<int>(position_.y),
        kCellSizeInt,
        kCellSizeInt};
    SDL_RenderCopyEx(&renderer, sprite_sheet_, &src_r, &dst_r, 0, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
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
    switch(direction_) {
        default:
        case EMovingDirection::DOWN:  return { 0,  1};
        case EMovingDirection::UP:    return { 0, -1};
        case EMovingDirection::LEFT:  return {-1,  0};
        case EMovingDirection::RIGHT: return { 1,  0};
    }
}

bool Ghost::IsOnChasingMode() const {
    return (mode_ == EMode::CHASING);
}

const Vec2& Ghost::GetPosition() const {
    return position_;
}

void Ghost::SwapToOpositeDirection() {
    switch(direction_) {
        case EMovingDirection::UP:    direction_ = EMovingDirection::DOWN; break;
        case EMovingDirection::DOWN:  direction_ = EMovingDirection::UP; break;
        case EMovingDirection::RIGHT: direction_ = EMovingDirection::LEFT; break;
        case EMovingDirection::LEFT:  direction_ = EMovingDirection::RIGHT; break;
    }
}