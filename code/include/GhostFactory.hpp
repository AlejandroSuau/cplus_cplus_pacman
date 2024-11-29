#pragma once

#include "utils/TextureManager.hpp"

#include "pathfinder/Pathfinder.hpp"

#include "Ghost.hpp"
#include "GameMap.hpp"

#include <memory>

class GhostFactory {
public:
    GhostFactory(
        TextureManager& texture_manager,
        Pathfinder& pathfinder,
        const GameMap& game_map);

    std::unique_ptr<Ghost> CreateGhostBlinky();
    std::unique_ptr<Ghost> CreateGhostInky();
    std::unique_ptr<Ghost> CreateGhostPinky();
    std::unique_ptr<Ghost> CreateGhostClyde();

private:
    TextureManager& texture_manager_;
    Pathfinder& pathfinder_;
    const GameMap& game_map_;
};