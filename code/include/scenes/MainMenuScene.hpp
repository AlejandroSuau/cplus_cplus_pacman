#pragma once

#include "utils/Renderer.hpp"
#include "utils/TextManager.hpp"
#include "utils/TextureManager.hpp"
#include "utils/CountdownTimer.hpp"
#include "utils/Vec2.hpp"

#include "Game.hpp"
#include "Constants.hpp"

#include "scenes/IScene.hpp"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <array>

struct Button {
    std::string text_;
    Vec2<int> text_coords_;
    SDL_Color text_color_;
    SDL_Color text_color_over_;
    SDL_Color text_color_click_;
    SDL_FRect rect_;
    bool is_over_ {false};
    bool is_pressed_ {false};

    void Reset() {
        is_over_ = false;
        is_pressed_ = false;
    }

    void Render(Renderer& renderer, TTF_Font& font) {
        SDL_Color color = kColorGray;
        if (is_pressed_) {
            color = kColorYellow;
        } else if (is_over_) {
            color = kColorWhite;
        }

        renderer.RenderRect(rect_);
        renderer.RenderText(font, text_, color, text_coords_.x, text_coords_.y);
    }
};



class MainMenuScene : public IScene {
public:
    MainMenuScene(
        Renderer& renderer,
        SoundManager& sound_manager,
        TextManager& text_manager,
        TextureManager& texture_manager);

    void Update(float dt) override;
    void Render() override;
    void OnEventKeyboard(EEventKeyboard event, SDL_Scancode scancode, Game& game) override;
    void OnEventMouse(EEventMouse event, const Vec2<float> coords, Game& game) override;

private:
    Renderer& renderer_;
    SoundManager& sound_manager_;
    TextManager& text_manager_;
    TextureManager& texture_manager_;

    CountdownTimer ghost_animation_timer_ {.1f};
    int ghost_animation_index_{0};

    CountdownTimer pacman_animation_timer_ {.1f};
    int pacman_animation_index_{0};

    CountdownTimer updating_dots_timer_ {.25f};
    std::size_t showing_dots_quantity_{1};
    const SDL_Rect dot_source {2, 182, 8, 8};
    std::array<SDL_FRect, 6> dots_ {
        SDL_FRect{550.f, 215.f, 4.f, 4.f},
        SDL_FRect{550.f, 215.f, 4.f, 4.f},
        SDL_FRect{550.f, 215.f, 4.f, 4.f},
        SDL_FRect{550.f, 215.f, 4.f, 4.f},
        SDL_FRect{550.f, 215.f, 4.f, 4.f},
        SDL_FRect{550.f, 215.f, 4.f, 4.f},
    };

    TTF_Font* font_title_;
    TTF_Font* font_text_;
    SDL_Texture* sprite_sheet_;

    Button button_play_ {
        "Play",
        Vec2<int>{360, 300},
        kColorGray,
        kColorWhite,
        kColorYellow,
        {325.f, 288.f, 70.f, 25.f}
    };

    Button button_exit_ {
        "Exit",
        Vec2<int>{360, 350},
        kColorGray,
        kColorWhite,
        kColorYellow,
        {325.f, 338.f, 70.f, 25.f}
    };

    void RenderGhosts();
    void RenderPacman();
    void RenderDots();

    void HandleMouseDown(const Vec2<float> coords);
    void HandleMouseUp(const Vec2<float> coords, Game& game);
    void HandleMouseMotion(const Vec2<float> coords);
};
