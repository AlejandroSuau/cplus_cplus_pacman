#include "utils/SpriteSheetAnimation.hpp"

#include "Constants.hpp"

#include <vector>

SpriteSheetAnimation::SpriteSheetAnimation(
    TextureManager& texture_manager,
    std::string texture_path,
    int width,
    int height,
    int sprite_width,
    int sprite_height,
    Uint64 sprite_swap_milliseconds) 
    : texture_manager_(texture_manager)
    , texture_path_(texture_path)
    , width_(width)
    , height_(height)
    , sprite_width_(sprite_width) 
    , sprite_height_(sprite_height) 
    , rows_count_(height_ / sprite_height_)
    , cols_count_(width_ / sprite_width_)
    , sprites_count_(rows_count_ * cols_count_)
    , sprite_swap_timer_(sprite_swap_milliseconds)
    , texture_(nullptr) {
    LoadTexture();
}

void SpriteSheetAnimation::LoadTexture() {
    std::string full_path = kAssetsFolderImages + texture_path_;
    texture_ = texture_manager_.LoadTexture(full_path);
}

SpriteSheetAnimation::~SpriteSheetAnimation() {
    if (texture_) SDL_DestroyTexture(texture_);
}

void SpriteSheetAnimation::Update(float dt) {
    if (sprite_swap_timer_.HasElapsed()) {
        sprite_index_ = (sprite_index_ + 1) % sprites_count_;
    }
}

void SpriteSheetAnimation::Render(SDL_Renderer& renderer) {
    SDL_Rect r {};
    // SDL_RenderCopyEx()
}
