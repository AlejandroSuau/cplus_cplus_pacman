#pragma once

#include <SDL2/SDL.h>

#include "utils/Vec2.hpp"
#include "utils/CountdownTimer.hpp"

#include "Constants.hpp"

#include <string>
#include <ranges>

template <std::size_t TexturesCount, const std::array<std::string, TexturesCount>& Paths>
class Animation {
public:
    Animation(
        TextureManager& texture_manager,
        Uint64 interval,
        Vec2 texture_dimensions)
        : texture_manager_(texture_manager)
        , timer_(interval)
        , texture_dimensions_(texture_dimensions)
        , rendering_texture_index_(0) {}

    ~Animation() {
        auto is_texture = [](SDL_Texture* tex) { return (tex != nullptr); };
        for (auto* texture : textures_ | std::views::filter(is_texture)) {
            SDL_DestroyTexture(texture);
        }
    }

    void Start() {
        rendering_texture_index_ = 0;
        timer_.Start();
    }

    void Update() {
        if (timer_.HasElapsed()) {
            rendering_texture_index_ = (rendering_texture_index_ + 1) % TexturesCount;
        }
    }

    void Render(SDL_Renderer& renderer, const Vec2& position) {
        SDL_FRect destination {position.x, position.y, texture_dimensions_.x, texture_dimensions_.y};
        SDL_RenderCopyF(&renderer, textures_[rendering_texture_index_], nullptr, &destination);
    }

    void LoadTextures() {
        for (std::size_t i = 0; i < TexturesCount; ++i) {
            std::string full_path = kAssetsFolderImages + Paths[i];
            textures_[i] = texture_manager_.LoadTexture(full_path);
            // SDL_Log("Animation: Loading texture: %s", full_path.c_str());
        }
    }

private:
    TextureManager& texture_manager_;
    CountdownTimer timer_;
    Vec2 texture_dimensions_;

    std::array<SDL_Texture*, TexturesCount> textures_;
    std::size_t rendering_texture_index_;
};
