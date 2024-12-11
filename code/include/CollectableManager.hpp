#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "utils/TextureManager.hpp"
#include "utils/Renderer.hpp"

#include "GameMap.hpp"
#include "Types.hpp"

#include <vector>
#include <memory>

enum class ECollectableType {
    SMALL,
    BIG
};

struct Collectable {
    ECollectableType type;
    unsigned int score;
    const SDL_FRect hitbox;
    bool is_marked_for_destroy;
    Collectable(ECollectableType type_, unsigned int score_, SDL_FRect hitbox_)
        : type(type_), score(score_), hitbox(hitbox_), is_marked_for_destroy(false) {}
};

class CollectableManager {
public:
    CollectableManager(
        Renderer& renderer,
        TextureManager& texture_manager,
        const GameMap& game_map);

    void CreateCollectables();
    void RemoveCollectablesMarkedForDestroy();
    void Render();
    void MarkAllForDestroy();
    bool DidCollectAll() const;
    CollectableList& GetCollectableList();
    
private:
    Renderer& renderer_;
    TextureManager& texture_manager_;
    const GameMap& game_map_;
    CollectableList collectables_;
    SDL_Texture* texture_;
};
