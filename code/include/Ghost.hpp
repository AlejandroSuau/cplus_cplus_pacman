#pragma once

#include <SDL2/SDL.h>

#include "utils/CountdownTimer.hpp"
#include "utils/Vec2.hpp"
#include "utils/TextureManager.hpp"

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
    
    void FindPath(int target_row, int target_col);

    void Update(float dt);
    void Render(SDL_Renderer& renderer);

private:
    TextureManager& texture_manager_;
    GameMap& game_map_;
    Pathfinder& pathfinder_;
    Vec2 position_;
    SDL_Color color_;
    Pathfinder::Path path_;
    std::size_t path_index_;
    CountdownTimer path_step_timer_;
};
