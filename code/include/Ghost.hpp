#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "utils/CountdownTimer.hpp"
#include "utils/Vec2.hpp"
#include "utils/TextureManager.hpp"
#include "utils/SpritesheetAnimation.hpp"

#include "GameMap.hpp"
#include "pathfinder/Pathfinder.hpp"

class Ghost {
public:
    Ghost(
        TextureManager& texture_manager,
        GameMap& game_map,
        Pathfinder& pathfinder,
        Vec2 position,
        SDL_Color color);
    
    virtual ~Ghost() = default;

    virtual void FindPath(int target_row, int target_col);
    void Update(float dt);
    void Render(SDL_Renderer& renderer);

    void ActivateVulnerability();

protected:
    enum class EMovingDirection {
        UP = 0,
        DOWN = 1,
        LEFT = 2,
        RIGHT = 3
    };

    enum class EType {
        RED = 0,
        PINKY = 1,
        BLUE = 2,
        YELLOW = 3
    };

    TextureManager& texture_manager_;
    GameMap& game_map_;
    Pathfinder& pathfinder_;
    Vec2 position_;
    SDL_Color color_;
    Pathfinder::Path path_;
    std::size_t path_index_;
    CountdownTimer path_step_timer_;
    EMovingDirection direction_;
    EType type_;
    bool is_vulnerable_;
    
    CountdownTimer animation_timer_;
    int sprite_index_;
    int sprites_count_;
    SDL_Texture* sprite_sheet_;

    void RenderPath(SDL_Renderer& renderer);
    SDL_Rect GetSourceRect() const;
};
