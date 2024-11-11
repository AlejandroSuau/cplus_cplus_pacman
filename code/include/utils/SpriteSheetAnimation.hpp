#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "utils/TextureManager.hpp"
#include "utils/CountdownTimer.hpp"

#include <vector>

class SpriteSheetAnimation {
public:
    SpriteSheetAnimation(
        TextureManager& texture_manager,
        std::string texture_path,
        int width,
        int height,
        int sprite_width,
        int sprite_height,
        Uint64 sprite_swap_milliseconds);
    ~SpriteSheetAnimation();

    void Update(float dt);
    void Render(SDL_Renderer& renderer);
    void LoadTexture();

private:
    TextureManager& texture_manager_;
    const std::string texture_path_;
    const int width_;
    const int height_;
    const int rows_count_;
    const int cols_count_;
    const int sprite_width_;
    const int sprite_height_;
    std::size_t sprites_count_;
    std::size_t sprite_index_;
    CountdownTimer sprite_swap_timer_;
    SDL_Texture* texture_;
};
