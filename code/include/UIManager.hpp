#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "utils/Renderer.hpp"
#include "utils/TextureManager.hpp"
#include "utils/TextManager.hpp"

#include "Player.hpp"
#include "Level.hpp"

class Game;

class UIManager {
public:
    UIManager(
        Renderer& renderer,
        TextManager& text_manager,
        TextureManager& texture_manager,
        const Player& player,
        const Level& level);
    
    void Render(SDL_Renderer& renderer, const Game& game);

private:
    Renderer& renderer_;
    TextManager& text_manager_;
    TextureManager& texture_manager_;
    const Player& player_;
    const Level& level_;
    
    TTF_Font* font_;
    SDL_Texture* sprite_sheet_;

    void LoadTextures();
};
