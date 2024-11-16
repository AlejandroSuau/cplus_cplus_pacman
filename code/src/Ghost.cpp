#include "Ghost.hpp"
#include "Constants.hpp"

// Inky Azul
// Target 2 casillas en direcc

Ghost::Ghost(
    TextureManager& texture_manager,
    GameMap& game_map,
    Pathfinder& pathfinder,
    Vec2 position,
    SDL_Color color)
    : texture_manager_(texture_manager)
    , game_map_(game_map)
    , pathfinder_(pathfinder)
    , position_(position)
    , color_(color)
    , path_index_(0)
    , path_step_timer_(500)
    , animation_timer_(100)
    , sprite_index_(0)
    , sprites_count_(2)
    , direction_(EMovingDirection::UP)
    , type_(EType::YELLOW)
    , is_vulnerable_(false) {

    sprite_sheet_ = texture_manager_.LoadTexture(kAssetsFolderImages + "spritesheet.png");
    // red
    /*.sprite {
	background: url('imgs/spritesheet.png') no-repeat -3px -83px;
	width: 154px;
	height: 14px;
    }*/

   // pink
   /*.sprite {
	background: url('imgs/spritesheet.png') no-repeat -3px -103px;
	width: 154px;
	height: 14px;
    }*/

   // blue
   /*.sprite {
	background: url('imgs/spritesheet.png') no-repeat -3px -123px;
	width: 154px;
	height: 14px;
    }*/

   // orange
   /*.sprite {
	background: url('imgs/spritesheet.png') no-repeat -3px -143px;
	width: 154px;
	height: 14px;
    }*/

   // vulnerable
   /*.sprite {
	background: url('imgs/spritesheet.png') no-repeat -3px -163px;
	width: 74px;
	height: 14px;
    }*/

   // dying player
   /*.sprite {
	background: url('imgs/spritesheet.png') no-repeat -3px -245px;
	width: 212px;
	height: 13px;
    }*/

   // player moving
   /*.sprite {
	background: url('imgs/spritesheet.png') no-repeat -4px -3px;
	width: 52px;
	height: 13px;
    }*/

   // COIN
   /*.sprite {
	background: url('imgs/spritesheet.png') no-repeat -2px -182px;
	width: 8px;
	height: 8px;
    }*/

   // GAMEOVER
   /*.sprite {
	background: url('imgs/spritesheet.png') no-repeat -13px -192px;
	width: 79px;
	height: 7px;
    }*/
}

void Ghost::FindPath(int target_row, int target_col) {
    path_index_ = 0;
    const auto [row, col] = game_map_.FromCoordsToRowCol(
        static_cast<int>(position_.x),
        static_cast<int>(position_.y));
    path_ = pathfinder_.FindPath(
        row,
        col,
        target_row,
        target_col);
}

void Ghost::Update(float dt) {
    if (animation_timer_.HasElapsed()) {
        sprite_index_ = (sprite_index_ + 1) % sprites_count_;
    }

    if (!path_.empty() && path_step_timer_.HasElapsed()) {
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

void Ghost::ActivateVulnerability() {
    is_vulnerable_ = true;
    // render el otro
    // comenzar el tempor
    // Si han pasado X segundos, cambiamos la imagen durante 1s. luego vuelve y luego vuelve a hacerlo
    // despues termina vulnerability

    // igual pero 163
}

void Ghost::Render(SDL_Renderer& renderer) {
    RenderPath(renderer);
    
    // Render animation
    auto src_r = GetSourceRect();
    SDL_Rect dst_r {
        static_cast<int>(position_.x),
        static_cast<int>(position_.y),
        kCellSizeInt,
        kCellSizeInt};
    SDL_RenderCopyEx(&renderer, sprite_sheet_, &src_r, &dst_r, 0, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
}

void Ghost::RenderPath(SDL_Renderer& renderer) {
    for (std::size_t i = path_index_; i < path_.size(); ++i) {
        const auto& cell = path_[i];
        SDL_Rect cell_rect {
            cell.second * kCellSizeInt,
            cell.first * kCellSizeInt,
            kCellSizeInt,
            kCellSizeInt};
        
        SDL_SetRenderDrawColor(&renderer, color_.r, color_.g, color_.b, 50);
        SDL_RenderFillRect(&renderer, &cell_rect);
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
