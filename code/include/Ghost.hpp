#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "utils/CountdownTimer.hpp"
#include "utils/Vec2.hpp"
#include "utils/TextureManager.hpp"
#include "utils/SpritesheetAnimation.hpp"

#include "GameMap.hpp"
#include "GhostMovementPatterns.hpp"

class Game;

class Ghost {
public:
    enum class EType {
        RED = 0,
        PINK = 1,
        BLUE = 2,
        YELLOW = 3
    };

    enum class EMovingDirection {
        UP = 0,
        DOWN = 1,
        LEFT = 2,
        RIGHT = 3
    };
    
    Ghost(
        TextureManager& texture_manager,
        const GameMap& game_map,
        std::string name,
        EType type,
        int x,
        int y,
        EMovingDirection direction,
        PathfindingPattern pathfinding_pattern);

    void FindPath(Game& game);

    void Update(float dt);
    void Render(SDL_Renderer& renderer);

    void ActivateVulnerability();

private:
    TextureManager& texture_manager_;
    const GameMap& game_map_;
    const std::string name_;
    EType type_;
    Vec2 position_;
    EMovingDirection direction_;
    PathfindingPattern patfinder_pattern_;
    bool is_vulnerable_;

    Pathfinder::Path path_;
    std::size_t path_index_;
    CountdownTimer path_step_timer_;
    
    CountdownTimer animation_timer_;
    int sprite_index_;
    int sprites_count_;
    SDL_Texture* sprite_sheet_;

    void RenderPath(SDL_Renderer& renderer);
    SDL_Rect GetSourceRect() const;
};
