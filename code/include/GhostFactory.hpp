#pragma once

#include "utils/TextureManager.hpp"

#include "pathfinder/Pathfinder.hpp"

#include "Ghost.hpp"
#include "GameMap.hpp"
#include "Level.hpp"

#include <memory>

class GhostFactory {
public:
    GhostFactory(
        Renderer& renderer,
        TextureManager& texture_manager,
        const GameMap& game_map,
        Pathfinder& pathfinder,
        const Level& level);

    std::unique_ptr<Ghost> CreateGhostBlinky();
    std::unique_ptr<Ghost> CreateGhostInky();
    std::unique_ptr<Ghost> CreateGhostPinky();
    std::unique_ptr<Ghost> CreateGhostClyde();

private:
    Renderer& renderer_;
    TextureManager& texture_manager_;
    const GameMap& game_map_;
    Pathfinder& pathfinder_;
    const Level& level_;
};