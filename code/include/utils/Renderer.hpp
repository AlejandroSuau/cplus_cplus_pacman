#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <string>

class Renderer {
public:
    Renderer(SDL_Renderer& renderer);

    void SetRenderingColor(const SDL_Color& color);
    void RenderRect(const SDL_FRect& rect);
    void RenderRectFilled(const SDL_FRect& rect);
    void RenderTexture(
        SDL_Texture* texture,
        const SDL_Rect& src_rect,
        const SDL_FRect& dst_rect,
        double angle = 0);
    void RenderText(
        TTF_Font& font,
        const std::string& text,
        SDL_Color color,
        int x,
        int y,
        bool centered = true);

private:
    SDL_Renderer* renderer_;
};
