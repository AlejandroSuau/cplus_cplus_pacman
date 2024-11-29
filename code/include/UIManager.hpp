#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "utils/TextureManager.hpp"
#include "utils/TextManager.hpp"

#include "Player.hpp"

class Game;

class UIManager {
public:
    UIManager(
        TextManager& text_manager,
        TextureManager& texture_manager,
        Player& player);
    
    void Render(SDL_Renderer& renderer, const Game& game);

private:
    TextManager& text_manager_;
    TextureManager& texture_manager_;
    Player& player_;
    
    TTF_Font* font_;
    SDL_Texture* sprite_sheet_;

    void LoadTextures();
};
