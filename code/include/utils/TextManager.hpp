#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <map>
#include <string>
#include <stdexcept>

class TextManager {
public:
    TTF_Font* LoadFont(const std::string& file_path, int font_size);
    
    void RemoveFont(const std::string& file_path);
    void RenderText(SDL_Renderer& renderer, TTF_Font& font, const std::string& text, SDL_Color color, int x, int y, bool centered = true);

private:
    std::map<std::string, TTF_Font*> fonts_;

    void ClearAllFonts();
};
