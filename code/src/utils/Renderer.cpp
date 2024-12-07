#include "utils/Renderer.hpp"

#include <stdexcept>

Renderer::Renderer(SDL_Renderer& renderer)
    : renderer_(&renderer) {}

void Renderer::SetRenderingColor(const SDL_Color& color) {
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
}

void Renderer::RenderRect(const SDL_FRect& rect) {
    SDL_RenderFillRectF(renderer_, &rect);
}

void Renderer::RenderRectFilled(const SDL_FRect& rect) {
    SDL_RenderFillRectF(renderer_, &rect);
}

void Renderer::RenderTexture(
    SDL_Texture* texture,
    const SDL_Rect& src_rect,
    const SDL_FRect& dst_rect,
    double angle) {
    SDL_RenderCopyExF(renderer_, texture, &src_rect, &dst_rect, angle, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
}

void Renderer::RenderText(
    TTF_Font& font,
    const std::string& text,
    SDL_Color color,
    int x,
    int y,
    bool centered) {
    SDL_Surface* text_surface = TTF_RenderText_Blended(&font, text.c_str(), color);
    if (!text_surface) {
        throw std::runtime_error("Failed to create text surface: " + std::string(TTF_GetError()));
    }
    
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer_, text_surface);
    if (!text_surface) {
        SDL_FreeSurface(text_surface);
        throw std::runtime_error("Failed to create text texture: " + std::string(SDL_GetError()));
    }

    SDL_Rect text_rect {x, y, text_surface->w, text_surface->h};
    if (centered) {
        text_rect.x -= text_surface->w / 2;
        text_rect.y -= text_surface->h / 2;
    }

    SDL_RenderCopy(renderer_, text_texture, nullptr, &text_rect);

    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
}