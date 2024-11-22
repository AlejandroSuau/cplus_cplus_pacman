#include "GhostMovementPatterns.hpp"

#include "Game.hpp"

#include <iostream>
#include <cmath>

// Rules:
// Always try to chase the player.
Pathfinder::Path FindPathPatternBlinky(const Vec2& ghost_row_col, Game& game) {
    return {};
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
    const auto player_direction = game.GetPlayer().GetDirectionVector();
    
    static const int kCellsDistance = 2;
    const auto [imaginary_row, imaginary_col] = game_map.GetCloserWalkableRowCol(
        {col_player, row_player}, kCellsDistance, player_direction);
    
    auto ghost_blinky = game.GetGhost("Blinky");
    const auto blinky_position = ghost_blinky->get().GetPosition();
    const auto [blinky_row, blinky_col] = game_map.FromCoordsToRowCol(
        blinky_position.x, blinky_position.y);
    Vec2 target_row_col {
        2 * imaginary_col - blinky_col,
        2 * imaginary_row - blinky_row};
    
    const Vec2 ghost_target_diff = (target_row_col - ghost_row_col);
    Vec2 ghost_target_direction;
    if (ghost_target_diff.x > 0) {
        ghost_target_direction.x = 1;
    } else if (ghost_target_diff.x < 0) {
        ghost_target_direction.x = -1;
    }

    if (ghost_target_diff.y > 0) {
        ghost_target_direction.y = 1;
    } else if (ghost_target_diff.y < 0) {
        ghost_target_direction.y = -1;
    }

    // 1. Bajamos / Subimos hasta que lo metemos en el rango 0 < num_cols
    // 2. Bajamos 1 cada vez hasta que es walkable.

    // A veces se va a -infinito (podria pasar con el otro.) Dar otra vuelta
    /*bool flag = false;
    while (!game_map.AreRowColWalkable(target_row_col.y, target_row_col.x)) {
        if (flag) {
            target_row_col.x -= ghost_target_direction.x;            
        } else {
            target_row_col.y -= ghost_target_direction.y;
        }
        flag = !flag;
    }*/
    
    // Y 

    // 2 casillas en dirección Player (el más cercano posible)
    //
    //game_map.GetF

    /*static const int kCellsDistance = 4;
    target_row += player_direction.y * kCellsDistance;
    target_col += player_direction.x * kCellsDistance;
    while (!game_map.AreRowColWalkable(target_row, target_col)) {
        target_row -= player_direction.y;
        target_col -= player_direction.x;
    }*/

// Inky Azul
// Target: 
//          1- 2 casillas en direcc del player (punto imaginario)
//              Coge a blinky y trza una línea entre el y ese punto.
//              Distancia entre blinky y el punto. Y se la suma al punto.

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
    const auto [target_row, target_col] = game_map.GetCloserWalkableRowCol(
        {player_col, player_row}, kCellsDistance, player_direction);

    return pathfinder.FindPath(ghost_row_col.y, ghost_row_col.x, target_row, target_col);
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
    
    int target_row;
    int target_col;
    if (distance < kLimitDistance) {
        target_row = static_cast<int>(game_map.GetRowsCount()) - 1;
        target_col = 0;
    } else {
        target_row = row_player;
        target_col = col_player;
    }

    return pathfinder.FindPath(ghost_row_col.y, ghost_row_col.x, target_row, target_col);
}
