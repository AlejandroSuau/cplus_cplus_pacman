#include "GhostMovementPatterns.hpp"

#include "Game.hpp"

#include <iostream>
#include <cmath>
#include <algorithm>

// Rules:
// Always try to chase the player.
Pathfinder::Path FindPathPatternBlinky(const Vec2& ghost_row_col, Game& game) {
    auto& pathfinder = game.GetPathfinder();
    const auto& game_map = game.GetMap();
    const auto player_position = game.GetPlayer().GetPosition();
    const auto [row_player, col_player] = game_map.FromCoordsToRowCol(
        player_position.x, player_position.y);
    
    return pathfinder.FindPath(ghost_row_col.y, ghost_row_col.x, row_player, col_player);
}

// Rules:
// 1. Imaginary point 2 cells through player's direction.
// 2. Then: Target = (2 * Imaginary Point) - Blinky's position)
Pathfinder::Path FindPathPatternInky(const Vec2& ghost_row_col, Game& game) {
    return {};
    auto& pathfinder = game.GetPathfinder();
    const auto& game_map = game.GetMap();
    const auto player_position = game.GetPlayer().GetPosition();
    const auto [player_row, player_col] = game_map.FromCoordsToRowCol(
        player_position.x, player_position.y);
    const auto player_direction = game.GetPlayer().GetDirectionVector();
    
    // Imaginary row col 2 cells through player's direction.
    static const int kCellsDistance = 2;
    Vec2 imaginary_row_col {
        player_col + (player_direction.x * kCellsDistance),
        player_row + (player_direction.y * kCellsDistance),
    };
    
    // Blinky position doubles that imaginary point diff.
    auto ghost_blinky = game.GetGhost("Blinky");
    const auto blinky_position = ghost_blinky->get().GetPosition();
    const auto [blinky_row, blinky_col] = game_map.FromCoordsToRowCol(
        blinky_position.x, blinky_position.y);
    Vec2 target_row_col {
        2 * imaginary_row_col.x - blinky_col,
        2 * imaginary_row_col.y - blinky_row};
    game_map.ClampRowColIntoMapDimensions(target_row_col);

    return pathfinder.FindPath(ghost_row_col.y, ghost_row_col.x, target_row_col.y, target_row_col.x);
}

// Rules:
// 4 tiles facing player's direction.
Pathfinder::Path FindPathPatternPinky(const Vec2& ghost_row_col, Game& game) {
    return {};
    auto& pathfinder = game.GetPathfinder();
    const auto& game_map = game.GetMap();
    const auto player_position = game.GetPlayer().GetPosition();
    auto [player_row, player_col] = game_map.FromCoordsToRowCol(
        player_position.x, player_position.y);
    const auto player_direction = game.GetPlayer().GetDirectionVector();

    static const int kCellsDistance = 4;
    Vec2 target_row_col {
        player_col + (player_direction.x * kCellsDistance),
        player_row + (player_direction.y * kCellsDistance),
    };
    game_map.ClampRowColIntoMapDimensions(target_row_col);

    return pathfinder.FindPath(ghost_row_col.y, ghost_row_col.x, target_row_col.y, target_row_col.x);
}

// Rules:
// If player is farder or equal to a euclidean distance of kLimitDistance, then try to chase player.
// Otherwise, go to the bottom left corner.
Pathfinder::Path FindPathPatternClyde(const Vec2& ghost_row_col, Game& game) {
    return {};
    auto& pathfinder = game.GetPathfinder();
    const auto& game_map = game.GetMap();
    const auto player_position = game.GetPlayer().GetPosition();
    const auto [row_player, col_player] = game_map.FromCoordsToRowCol(
        player_position.x, player_position.y);
    
    static const double kLimitDistance = 8.0;
    const auto distance = std::hypot(
        ghost_row_col.y - row_player,
        ghost_row_col.x - col_player);
    
    Vec2 target_row_col;
    if (distance < kLimitDistance) {
        target_row_col.y = static_cast<int>(game_map.GetRowsCount()) - 1;
    } else {
        target_row_col.x = col_player;
        target_row_col.y = row_player;
    }

    return pathfinder.FindPath(ghost_row_col.y, ghost_row_col.x, target_row_col.y, target_row_col.x);
}
