#include "scenes/MainMenuScene.hpp"

#include "Constants.hpp"
#include "Game.hpp"

#include "utils/Vec2.hpp"
#include "utils/Collisions.hpp"

#include <array>
#include <algorithm>

MainMenuScene::MainMenuScene(
    Renderer& renderer,
    SoundManager& sound_manager,
    TextManager& text_manager,
    TextureManager& texture_manager) 
    : renderer_(renderer)
    , sound_manager_(sound_manager)
    , text_manager_(text_manager)
    , texture_manager_(texture_manager) {
    font_title_ = text_manager_.LoadFont(kAssetsFolderFonts + "atari-full.ttf", 24, "atari-big");
    font_text_ = text_manager_.LoadFont(kAssetsFolderFonts + "atari-full.ttf", 14, "atari-small");
    sprite_sheet_ = texture_manager_.LoadTexture(kAssetsFolderImages + "spritesheet.png");
}

void MainMenuScene::Update(float dt) {
    ghost_animation_timer_.Update(dt);
    if (ghost_animation_timer_.DidFinish()) {
        ghost_animation_index_ = !ghost_animation_index_;
    }

    pacman_animation_timer_.Update(dt);
    if (pacman_animation_timer_.DidFinish()) {
        pacman_animation_index_ = ++pacman_animation_index_ % 3;
    }

    updating_dots_timer_.Update(dt);
    if (updating_dots_timer_.DidFinish()) {
        showing_dots_quantity_ = std::clamp(showing_dots_quantity_, ++showing_dots_quantity_, dots_.size());
    }

    std::size_t i = 0;
    while (i < showing_dots_quantity_) {
        auto& d = dots_[i];
        d.x -= 100.f * dt;
        if (d.x <= 400.f) d.x = 550.f;
        ++i;
    }
}

void MainMenuScene::Render() {
    renderer_.RenderText(*font_title_, "Pac-Man Clone", kColorWhite, 360, 100);

    renderer_.SetRenderingColor(kColorGray);
    renderer_.RenderRect({125.f, 180.f, 450.f, 70.f});

    RenderGhosts();
    RenderPacman();
    RenderDots();

    button_play_.Render(renderer_, *font_text_);
    button_exit_.Render(renderer_, *font_text_);
}

void MainMenuScene::RenderGhosts() {
    static int kGhostPadding = 6;
    SDL_FRect dest_rect {150, 200, 28, 28};
    static const std::array<SDL_Rect, 4> ghost_source_rects {
        SDL_Rect{123, 83, 14, 14},   // red
        SDL_Rect{123, 103, 14, 14},  // pink
        SDL_Rect{123, 123, 14, 14},  // blue
        SDL_Rect{123, 143, 14, 14}}; // yellow
    for (auto& rect : ghost_source_rects) {
        auto r = rect;
        r.x = rect.x + (rect.w + kGhostPadding) * ghost_animation_index_;
        renderer_.RenderTexture(sprite_sheet_, r, dest_rect);
        
        dest_rect.x += dest_rect.w + 10.f;
    }
}

void MainMenuScene::RenderPacman() {
    static const int kPacmanPadding = 8;
    SDL_Rect source_rect {282, 2, 32, 32};
    source_rect.y += (source_rect.h + kPacmanPadding) * pacman_animation_index_;
    
    SDL_FRect dest_rect {375, 200, 32, 32};
    renderer_.RenderTexture(sprite_sheet_, source_rect, dest_rect);
}

void MainMenuScene::RenderDots() {
    for (std::size_t i = 0; i < showing_dots_quantity_; ++i) {
        renderer_.RenderTexture(sprite_sheet_, dot_source, dots_[i]);
    }
}

void MainMenuScene::OnEventKeyboard(EEventKeyboard event, SDL_Scancode scancode, Game& game) {}

void MainMenuScene::OnEventMouse(EEventMouse event, const Vec2<float> coords, Game& game) {
    switch(event) {
        case EEventMouse::DOWN:   HandleMouseDown(coords);      break;
        case EEventMouse::UP:     HandleMouseUp(coords, game);  break;
        case EEventMouse::MOTION: HandleMouseMotion(coords);    break;
    }
}

void MainMenuScene::HandleMouseDown(const Vec2<float> coords) {
    button_play_.is_pressed_ = IsPointInsideRect(coords, button_play_.rect_);
    button_exit_.is_pressed_ = IsPointInsideRect(coords, button_exit_.rect_);
}

void MainMenuScene::HandleMouseUp(const Vec2<float> coords, Game& game) {
    if (button_play_.is_pressed_ && IsPointInsideRect(coords, button_play_.rect_)) {
        game.SwapToGameScene();
    } else if (button_exit_.is_pressed_ && IsPointInsideRect(coords, button_exit_.rect_)) {
        game.Shutdown();
    }
    button_play_.is_pressed_ = false;
    button_exit_.is_pressed_ = false;
}

void MainMenuScene::HandleMouseMotion(const Vec2<float> coords) {
    if (IsPointInsideRect(coords, button_play_.rect_)) {
        button_play_.is_over_ = true;
    } else {
        button_play_.Reset();
    }

    if (IsPointInsideRect(coords, button_exit_.rect_)) {
        button_exit_.is_over_ = true;
    } else {
        button_exit_.Reset();
    }
}
