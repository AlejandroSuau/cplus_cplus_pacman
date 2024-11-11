#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "utils/TextureManager.hpp"

#include "ScoreManager.hpp"

class Game;

class UIManager {
public:
    UIManager(TextureManager& texture_manager, ScoreManager& score_manager);
    
    void Render(SDL_Renderer& renderer, const Game& game);

private:
    TextureManager& texture_manager_;
    ScoreManager& score_manager_;
    
    SDL_Texture* tutorial_texture_;
    SDL_Texture* gameover_texture_;

    void LoadTextures();
};
