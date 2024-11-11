#include "UIManager.hpp"

#include "Constants.hpp"
#include "Game.hpp"

UIManager::UIManager(TextureManager& texture_manager, ScoreManager& score_manager)
    : texture_manager_(texture_manager)
    , score_manager_(score_manager)
    , tutorial_texture_(nullptr)
    , gameover_texture_(nullptr) {
    LoadTextures();
}

void UIManager::LoadTextures() {
    //tutorial_texture_ = texture_manager_.LoadTexture(kAssetsFolderImages + "message.png");
    //gameover_texture_ = texture_manager_.LoadTexture(kAssetsFolderImages + "gameover.png");
}

void UIManager::Render(SDL_Renderer& renderer, const Game& game) {
    /*if (game.IsReadyToPlay()) {
        SDL_RenderCopyF(&renderer, tutorial_texture_, nullptr, &kTextureRectTutorial);
    } else {
        score_manager_.Render(renderer);   
    }

    if (game.IsFinishing() || game.IsFinished()) {
        SDL_RenderCopyF(&renderer, gameover_texture_, nullptr, &kTextureRectGameOver);
    }*/
}