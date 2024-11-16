#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "utils/TextureManager.hpp"

#include "GameMap.hpp"
#include "ScoreManager.hpp"

#include <vector>
#include <memory>

class Game;

class CollectableList {
public:
    CollectableList(
        TextureManager& texture_manager,
        ScoreManager& score_manager,
        const GameMap& game_map);

    void Init();
    void ProcessCollisions(Game& game);
    void RemoveCollectablesMarkedForDestroy();

    void Render(SDL_Renderer& renderer);

private:
    enum class ECollectableType {
        SMALL,
        BIG
    };
    struct Collectable {
        ECollectableType type;
        unsigned int score;
        int x;
        int y;
        int w;
        int h;
        bool is_marked_for_destroy;
    };

    TextureManager& texture_manager_;
    ScoreManager& score_manager_;
    const GameMap& game_map_;
    
    std::vector<std::unique_ptr<Collectable>> collectables_;
    SDL_Texture* texture_;

    void OnCollision(Collectable& collectable);
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
