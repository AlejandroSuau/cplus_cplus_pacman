#include "UIManager.hpp"

#include "Constants.hpp"
#include "scenes/GameScene.hpp"

namespace {
static const SDL_Color kWhiteColor {255, 255, 255, 255};
}

UIManager::UIManager(
    Renderer& renderer,
    TextManager& text_manager,
    TextureManager& texture_manager,
    const Player& player,
    const Level& level)
    : renderer_(renderer)
    , text_manager_(text_manager)
    , texture_manager_(texture_manager)
    , player_(player)
    , level_(level)
    , font_(nullptr)
    , sprite_sheet_(nullptr) {
    LoadTextures();
}

void UIManager::LoadTextures() {
    font_ = text_manager_.LoadFont(kAssetsFolderFonts + "atari-full.ttf", 14);
    sprite_sheet_ = texture_manager_.LoadTexture(kAssetsFolderImages + "spritesheet.png");
}

void UIManager::Render(const GameScene& game_scene) {
    renderer_.RenderText(*font_, "Score", kWhiteColor, 145, 45);
    const auto score_string = std::to_string(player_.GetScore());
    renderer_.RenderText(*font_, score_string, kWhiteColor, 143, 75);

    renderer_.RenderText(*font_, "Level", kWhiteColor, 372, 45);
    const auto level_string = std::to_string(level_.GetNumber());
    renderer_.RenderText(*font_, level_string, kWhiteColor, 372, 75);

    // Game Status
    // GAME READY
    if (game_scene.IsReadyToPlay()) {
        const SDL_Rect src_r_ready {203, 2, 46, 7};
        const SDL_FRect dst_r_ready {
            kGamePaddingX + kGameWidth / 2 - 40,
            kGamePaddingY + kGameHeight / 2 + 40,
            src_r_ready.w * 2.f,
            src_r_ready.h * 2.f};
        renderer_.RenderTexture(sprite_sheet_, src_r_ready, dst_r_ready);
    }

    // GAME OVER
    if (game_scene.IsGameOver()) {
        const SDL_Rect src_r_gameover {13, 192, 79, 7};
        const SDL_FRect dst_r_gameover {
            kGamePaddingX + kGameWidth / 2 - 77,
            kGamePaddingY + kGameHeight / 2 + 40,
            src_r_gameover.w * 2.f,
            src_r_gameover.h * 2.f};
        renderer_.RenderTexture(sprite_sheet_, src_r_gameover, dst_r_gameover);
    }

    // Player lifes.
    const SDL_Rect src_r_lifes {85, 164, 10, 11};
    const auto lifes = game_scene.GetPlayer().GetLifes();
    static const int life_width = 10 * 2;
    static const int life_height = 11 * 2;
    static const int life_padding = 10;
    int current_x = kGamePaddingX + 15;
    for (unsigned int i = 0; i < lifes; ++i) {
        const SDL_FRect dst_r {
            current_x,
            kGameHeight + kGamePaddingY + 15,
            life_width,
            life_width};
        renderer_.RenderTexture(sprite_sheet_, src_r_lifes, dst_r);
        current_x += life_width + life_padding;
    }
}