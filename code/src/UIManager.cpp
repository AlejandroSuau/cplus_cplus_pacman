#include "UIManager.hpp"

#include "Constants.hpp"
#include "Game.hpp"

namespace {
static const SDL_Color kWhiteColor {255, 255, 255, 255};
}

UIManager::UIManager(
    TextManager& text_manager,
    TextureManager& texture_manager,
    ScoreManager& score_manager)
    : text_manager_(text_manager)
    , texture_manager_(texture_manager)
    , score_manager_(score_manager)
    , font_(nullptr)
    , tutorial_texture_(nullptr)
    , gameover_texture_(nullptr) {
    font_ = text_manager.LoadFont(kAssetsFolderFonts + "atari-full.ttf", 18);
}

void UIManager::LoadTextures() {
    //tutorial_texture_ = texture_manager_.LoadTexture(kAssetsFolderImages + "message.png");
    //gameover_texture_ = texture_manager_.LoadTexture(kAssetsFolderImages + "gameover.png");
}

void UIManager::Render(SDL_Renderer& renderer, const Game& game) {
    text_manager_.RenderText(renderer, *font_, "Score", kWhiteColor, 110, 45);
    
    const auto score_string = std::to_string(score_manager_.GetScore());
    text_manager_.RenderText(renderer, *font_, score_string, kWhiteColor, 145, 75);
    /*if (game.IsReadyToPlay()) {
        SDL_RenderCopyF(&renderer, tutorial_texture_, nullptr, &kTextureRectTutorial);
    } else {
        score_manager_.Render(renderer);   
    }

    if (game.IsFinishing() || game.IsFinished()) {
        SDL_RenderCopyF(&renderer, gameover_texture_, nullptr, &kTextureRectGameOver);
    }*/
}