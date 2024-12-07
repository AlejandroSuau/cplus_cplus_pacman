#pragma once

#include "utils/Renderer.hpp"
#include "utils/TextManager.hpp"
#include "utils/TextureManager.hpp"
#include "utils/CountdownTimer.hpp"

#include "scenes/IScene.hpp"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class MainMenuScene : public IScene {
public:
    MainMenuScene(
        Renderer& renderer,
        TextManager& text_manager,
        TextureManager& texture_manager);

    void Update(float dt) override;
    void Render() override;

private:
    Renderer& renderer_;
    TextManager& text_manager_;
    TextureManager& texture_manager_;

    CountdownTimer ghost_animation_timer_ {.1f};
    int ghost_animation_index_{0};

    TTF_Font* font_title_;
    TTF_Font* font_text_;
    SDL_Texture* sprite_sheet_;
    // Texture
    // Text render
    // Fuente para el titulo
    // Fuente para los botones
    // On Hover each ghost show color name instead of title.
    // Create hover listener
    // On click yellow
    void RenderGhosts();
    void RenderButtons();
};

// Hover yellow
/*
yellow 255, 205, 50

blue 51, 255, 255

pink 254, 152, 204

red 254, 0, 1

pacman 255 250 1
*/