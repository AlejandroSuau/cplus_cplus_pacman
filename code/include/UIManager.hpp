#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "utils/TextureManager.hpp"
#include "utils/TextManager.hpp"

#include "ScoreManager.hpp"

class Game;

class UIManager {
public:
    UIManager(
        TextManager& text_manager,
        TextureManager& texture_manager,
        ScoreManager& score_manager);
    
    void Render(SDL_Renderer& renderer, const Game& game);

private:
    TextManager& text_manager_;
    TextureManager& texture_manager_;
    ScoreManager& score_manager_;
    
    TTF_Font* font_;
    SDL_Texture* tutorial_texture_;
    SDL_Texture* gameover_texture_;

    void LoadTextures();
};
