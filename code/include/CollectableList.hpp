#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "utils/TextureManager.hpp"

#include "GameMap.hpp"
#include "ScoreManager.hpp"

#include <vector>
#include <memory>
#include <optional>

class Game;

class CollectableList {
public:
    enum class ECollectableType {
        SMALL,
        BIG
    };
    CollectableList(
        TextureManager& texture_manager,
        ScoreManager& score_manager,
        const GameMap& game_map);

    void Init();
    std::optional<ECollectableType> ProcessCollisions(Game& game);
    void RemoveCollectablesMarkedForDestroy();

    void Render(SDL_Renderer& renderer);

private:
    struct Collectable {
        ECollectableType type;
        unsigned int score;
        int x;
        int y;
        int w;
        int h;
        bool is_marked_for_destroy;
        Collectable(ECollectableType type_, unsigned int score_, int x_, int y_, int w_, int h_)
            : type(type_), score(score_), x(x_), y(y_), w(w_), h(h_), is_marked_for_destroy(false) {}
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
