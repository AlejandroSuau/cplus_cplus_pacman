#include "GhostMovementPatterns.hpp"

#include "Game.hpp"

Pathfinder::Path FindPathPatternBlinky(const Vec2& ghost_row_col, Game& game) {
    auto& pathfinder = game.GetPathfinder();
    const auto& game_map = game.GetMap();
    const auto player_position = game.GetPlayer().GetPosition();
    const auto [row_player, col_player] = game_map.FromCoordsToRowCol(
        player_position.x, player_position.y);
    
    return pathfinder.FindPath(ghost_row_col.y, ghost_row_col.x, row_player, col_player);
}

Pathfinder::Path FindPathPatternInky(const Vec2& ghost_row_col, Game& game) {
    auto& pathfinder = game.GetPathfinder();
    const auto& game_map = game.GetMap();
    const auto player_position = game.GetPlayer().GetPosition();
    const auto [row_player, col_player] = game_map.FromCoordsToRowCol(
        player_position.x, player_position.y);
    
    return pathfinder.FindPath(ghost_row_col.y, ghost_row_col.x, row_player, col_player);
}

Pathfinder::Path FindPathPatternPinky(const Vec2& ghost_row_col, Game& game) {
    auto& pathfinder = game.GetPathfinder();
    const auto& game_map = game.GetMap();
    const auto player_position = game.GetPlayer().GetPosition();
    const auto [row_player, col_player] = game_map.FromCoordsToRowCol(
        player_position.x, player_position.y);
    
    return pathfinder.FindPath(ghost_row_col.y, ghost_row_col.x, row_player, col_player);
}

Pathfinder::Path FindPathPatternClyde(const Vec2& ghost_row_col, Game& game) {
    auto& pathfinder = game.GetPathfinder();
    const auto& game_map = game.GetMap();
    const auto player_position = game.GetPlayer().GetPosition();
    const auto [row_player, col_player] = game_map.FromCoordsToRowCol(
        player_position.x, player_position.y);
    
    return pathfinder.FindPath(ghost_row_col.y, ghost_row_col.x, row_player, col_player);
}
