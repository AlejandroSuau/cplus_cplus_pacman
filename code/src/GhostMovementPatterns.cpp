#include "GhostMovementPatterns.hpp"

#include "Game.hpp"

#include <iostream>
#include <cmath>
#include <algorithm>

// Rules:
// Always try to chase the player.
Pathfinder::Path FindPathPatternBlinky(const Vec2<int>& col_row_ghost, Game& game) {
    auto& pathfinder = game.GetPathfinder();
    const auto& game_map = game.GetMap();
    const auto player_position = game.GetPlayer().GetCenterPosition();
    const auto col_row_player = game_map.FromCoordsToColRow(player_position);
    
    return pathfinder.FindPath(col_row_ghost, col_row_player);
}

// Rules:
// 1. Imaginary point 2 cells through player's direction.
// 2. Then: Target = (2 * Imaginary Point) - Blinky's position)
Pathfinder::Path FindPathPatternInky(const Vec2<int>& col_row_ghost, Game& game) {
    auto& pathfinder = game.GetPathfinder();
    const auto& game_map = game.GetMap();
    const auto player_position = game.GetPlayer().GetCenterPosition();
    const auto row_col_player = game_map.FromCoordsToColRow(player_position);
    const auto player_direction = game.GetPlayer().GetDirectionVector();
    
    // Imaginary row col 2 cells through player's direction.
    static const int kCellsDistance = 2;
    Vec2<int> row_col_imaginary {
        row_col_player.x + (player_direction.x * kCellsDistance),
        row_col_player.y + (player_direction.y * kCellsDistance)
    };
    
    // Blinky position doubles that imaginary point diff.
    auto ghost_blinky = game.GetGhost("Blinky");
    const auto blinky_position = ghost_blinky->get().GetCenterPosition();
    const auto col_row_blinky = game_map.FromCoordsToColRow(blinky_position);
    Vec2<int> col_row_target {
        2 * row_col_imaginary.x - col_row_blinky.x,
        2 * row_col_imaginary.y - col_row_blinky.y};
    game_map.ClampColRowIntoMapDimensions(col_row_target);

    return pathfinder.FindPath(col_row_ghost, col_row_target);
}

// Rules:
// 4 tiles facing player's direction.
Pathfinder::Path FindPathPatternPinky(const Vec2<int>& col_row_ghost, Game& game) {
    auto& pathfinder = game.GetPathfinder();
    const auto& game_map = game.GetMap();
    const auto player_position = game.GetPlayer().GetCenterPosition();
    auto col_row_player = game_map.FromCoordsToColRow(player_position);
    const auto player_direction = game.GetPlayer().GetDirectionVector();

    static const int kCellsDistance = 4;
    Vec2<int> col_row_target {
        col_row_player.x + (player_direction.x * kCellsDistance),
        col_row_player.y + (player_direction.y * kCellsDistance),
    };
    game_map.ClampColRowIntoMapDimensions(col_row_target);

    return pathfinder.FindPath(col_row_ghost, col_row_target);
}

// Rules:
// If player is farder or equal to a euclidean distance of kLimitDistance, then try to chase player.
// Otherwise, go to the bottom left corner.
Pathfinder::Path FindPathPatternClyde(const Vec2<int>& col_row_ghost, Game& game) {
    auto& pathfinder = game.GetPathfinder();
    const auto& game_map = game.GetMap();
    const auto player_position = game.GetPlayer().GetCenterPosition();
    const auto col_row_player = game_map.FromCoordsToColRow(player_position);
    
    static const double kLimitDistance = 8.0;
    const auto distance = std::hypot(
        col_row_ghost.y - col_row_player.y,
        col_row_ghost.x - col_row_player.x);
    
    Vec2<int> col_row_target;
    if (distance < kLimitDistance) {
        col_row_target.y = static_cast<int>(game_map.GetRowsCount()) - 1;
    } else {
        col_row_target = col_row_player;
    }

    return pathfinder.FindPath(col_row_ghost, col_row_target);
}
