#include "utils/Renderer.hpp"


Renderer::Renderer(SDL_Renderer& renderer)
    : renderer_(&renderer) {}

void Renderer::SetRenderingColor(const SDL_Color& color) {
    SDL_SetRenderDrawColor(renderer_, color.r, color.g, color.b, color.a);
}

void Renderer::RenderRect(const SDL_Rect& rect) {
    SDL_RenderFillRect(renderer_, &rect);
}

void Renderer::RenderRectFilled(const SDL_Rect& rect) {
    SDL_RenderFillRect(renderer_, &rect);
}

void Renderer::RenderTexture(
    SDL_Texture* texture,
    const SDL_Rect& src_rect,
    const SDL_Rect& dst_rect,
    double angle) {
    SDL_RenderCopyEx(renderer_, texture, &src_rect, &dst_rect, angle, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
}
