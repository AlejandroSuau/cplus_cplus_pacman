#include "UIManager.hpp"

#include "Constants.hpp"
#include "Game.hpp"

namespace {
static const SDL_Color kWhiteColor {255, 255, 255, 255};
}

UIManager::UIManager(
    TextManager& text_manager,
    TextureManager& texture_manager,
    Player& player)
    : text_manager_(text_manager)
    , texture_manager_(texture_manager)
    , player_(player)
    , font_(nullptr)
    , sprite_sheet_(nullptr) {
    LoadTextures();
}

void UIManager::LoadTextures() {
    font_ = text_manager_.LoadFont(kAssetsFolderFonts + "atari-full.ttf", 14);
    sprite_sheet_ = texture_manager_.LoadTexture(kAssetsFolderImages + "spritesheet.png");
}

void UIManager::Render(SDL_Renderer& renderer, const Game& game) {
    // Current score
    text_manager_.RenderText(renderer, *font_, "Score", kWhiteColor, 140, 45);
    const auto score_string = std::to_string(player_.GetScore());
    text_manager_.RenderText(renderer, *font_, score_string, kWhiteColor, 138, 75);

    text_manager_.RenderText(renderer, *font_, "Level", kWhiteColor, 350, 45);
    const auto high_score_string = std::to_string(player_.GetScore());
    text_manager_.RenderText(renderer, *font_, score_string, kWhiteColor, 350, 75);

    text_manager_.RenderText(renderer, *font_, "High Score", kWhiteColor, 580, 45);
    const auto level_string = std::to_string(player_.GetScore());
    text_manager_.RenderText(renderer, *font_, score_string, kWhiteColor, 580, 75);

    // Game Status
    // GAME READY
    if (game.IsReadyToPlay()) {
        const SDL_Rect src_r_ready {203, 2, 46, 7};
        const SDL_Rect dst_r_ready {
            kGamePaddingX + kGameWidth / 2 - 40,
            kGamePaddingY + kGameHeight / 2 + 40,
            src_r_ready.w * 2,
            src_r_ready.h * 2};
        SDL_RenderCopyEx(&renderer, sprite_sheet_, &src_r_ready, &dst_r_ready, 0, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
    }

    // GAME OVER
    if (game.IsGameOver()) {
        const SDL_Rect src_r_gameover {13, 192, 79, 7};
        const SDL_Rect dst_r_gameover {
            kGamePaddingX + kGameWidth / 2 - 77,
            kGamePaddingY + kGameHeight / 2 + 40,
            src_r_gameover.w * 2,
            src_r_gameover.h * 2};
        SDL_RenderCopyEx(&renderer, sprite_sheet_, &src_r_gameover, &dst_r_gameover, 0, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
    }

    // Player lifes.
    const SDL_Rect src_r_lifes {85, 164, 10, 11};
    const auto lifes = game.GetPlayer().GetLifes();
    static const int life_width = 10 * 2;
    static const int life_height = 11 * 2;
    static const int life_padding = 10;
    int current_x = kGamePaddingX + 15;
    for (unsigned int i = 0; i < lifes; ++i) {
        const SDL_Rect dst_r {
            current_x,
            kGameHeight + kGamePaddingY + 15,
            life_width,
            life_width};
        SDL_RenderCopyEx(&renderer, sprite_sheet_, &src_r_lifes, &dst_r, 0, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
        current_x += life_width + life_padding;
    }
}