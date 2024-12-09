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

    if (game_scene.IsReadyToPlay()) {
        renderer_.RenderTexture(sprite_sheet_, {203, 2, 46, 7}, {332.f, 460.f, 92.f, 14.f});
    } else if(game_scene.IsGameOver()) {
        renderer_.RenderTexture(sprite_sheet_, {13, 192, 79, 7}, {295.f, 460.f, 158.f, 14.f});
    }

    const auto lifes = game_scene.GetPlayer().GetLifes();
    float current_x = 115.f;
    for (unsigned int i = 0; i < lifes; ++i) {
        renderer_.RenderTexture(sprite_sheet_, {85, 164, 10, 11}, {current_x, 755.f, 20.f, 20.f});
        current_x += 35.f;
    }
}