#include "utils/TextManager.hpp"

TTF_Font* TextManager::LoadFont(const std::string& file_path, int font_size) {
    if (fonts_.count(file_path.c_str()) == 0) {
        TTF_Font* font = TTF_OpenFont(file_path.c_str(), font_size);
        if (!font) {
            SDL_Log("Failed to load font: %s. SDL Error: %s", file_path.c_str(), SDL_GetError());
            return nullptr;
        }
        fonts_[file_path.c_str()] = font;
    }

    return fonts_[file_path.c_str()];
}

void TextManager::RemoveFont(const std::string& file_path) {
    auto it = fonts_.find(file_path.c_str());
    if (it != fonts_.end()) {
        TTF_CloseFont(it->second);
        fonts_.erase(it);
    }
}

void TextManager::RenderText(SDL_Renderer& renderer, TTF_Font& font, const std::string& text, SDL_Color color, int x, int y) {
    SDL_Surface* text_surface = TTF_RenderText_Blended(&font, text.c_str(), color);
    if (!text_surface) {
        throw std::runtime_error("Failed to create text surface: " + std::string(TTF_GetError()));
    }
    
    SDL_Texture* text_texture = SDL_CreateTextureFromSurface(&renderer, text_surface);
    if (!text_surface) {
        SDL_FreeSurface(text_surface);
        throw std::runtime_error("Failed to create text texture: " + std::string(SDL_GetError()));
    }

    SDL_Rect text_rect {x, y, text_surface->w, text_surface->h};
    SDL_RenderCopy(&renderer, text_texture, nullptr, &text_rect);

    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
}

void TextManager::ClearAllFonts() {
    for (auto& [font_id, font] : fonts_) {
        TTF_CloseFont(font);
    }
    fonts_.clear();
}
