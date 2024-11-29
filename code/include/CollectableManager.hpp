#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "utils/TextureManager.hpp"

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
    const SDL_Rect hitbox;
    bool is_marked_for_destroy;
    
    Collectable(ECollectableType type_, unsigned int score_, int x_, int y_, int w_, int h_)
        : type(type_), score(score_), hitbox{x_, y_, w_, h_}, is_marked_for_destroy(false) {}
};

class CollectableManager {
public:
    CollectableManager(
        TextureManager& texture_manager,
        const GameMap& game_map);

    void Init();
    void RemoveCollectablesMarkedForDestroy();

    void Render(SDL_Renderer& renderer);

    CollectableList& GetCollectableList();
private:
    TextureManager& texture_manager_;
    const GameMap& game_map_;
    CollectableList collectables_;
    SDL_Texture* texture_;
};


/*class Collectable {
public:
    Collectable(
        ECollectableType collectable_type,
        Vec2 position,
        unsigned int score);
    virtual ~Collectable() = default;

    virtual void OnCollect();
    void Render(SDL_Renderer& renderer);

private:
    SDL_FRect rect_;
    unsigned int score_;
};

class CollectablePower : Collectable {

};*/
