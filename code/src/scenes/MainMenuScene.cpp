#include "scenes/MainMenuScene.hpp"

#include "Constants.hpp"

#include <array>

MainMenuScene::MainMenuScene(
    Renderer& renderer,
    TextManager& text_manager,
    TextureManager& texture_manager) 
    : renderer_(renderer)
    , text_manager_(text_manager)
    , texture_manager_(texture_manager) {
    font_title_ = text_manager_.LoadFont(kAssetsFolderFonts + "atari-full.ttf", 24);
    font_text_ = text_manager_.LoadFont(kAssetsFolderFonts + "atari-full.ttf", 14);
    sprite_sheet_ = texture_manager_.LoadTexture(kAssetsFolderImages + "spritesheet.png");
}

void MainMenuScene::Update(float dt) {
    ghost_animation_timer_.Update(dt);
    if (ghost_animation_timer_.DidFinish()) {
        ghost_animation_index_ = !ghost_animation_index_;
    }
}

void MainMenuScene::Render() {
    SDL_Color kWhiteColor {255, 255, 255, 0};
    renderer_.RenderText(*font_title_, "Pac-Man Clone", kWhiteColor, 350, 100);

    RenderGhosts();
}

void MainMenuScene::RenderGhosts() {
    static int kGhostPadding = 6;
    SDL_FRect dest_rect {150, 200, 28, 28};
    std::array<SDL_Rect, 4> ghost_source_rects {
        SDL_Rect{123, 83, 14, 14},   // red
        SDL_Rect{123, 103, 14, 14},  // pink
        SDL_Rect{123, 123, 14, 14},  // blue
        SDL_Rect{123, 143, 14, 14}}; // yellow
    for (auto& rect : ghost_source_rects) {
        rect.x += (rect.w + kGhostPadding) * ghost_animation_index_;
        renderer_.RenderTexture(sprite_sheet_, rect, dest_rect);
        
        dest_rect.x += dest_rect.w + 10.f;
    }
}
