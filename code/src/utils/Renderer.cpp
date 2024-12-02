#include "utils/Renderer.hpp"


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
