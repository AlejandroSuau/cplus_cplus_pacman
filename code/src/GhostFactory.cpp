#include "GhostFactory.hpp"

#include "Game.hpp"
#include "GhostMovementPatterns.hpp"

GhostFactory::GhostFactory(
    TextureManager& texture_manager,
    const GameMap& game_map) 
    : texture_manager_(texture_manager)
    , game_map_(game_map) {}

std::unique_ptr<Ghost> GhostFactory::CreateGhostBlinky() {
    const auto [x, y] = game_map_.FromRowColToCoords(6, 8);
    return std::make_unique<Ghost>(
        texture_manager_,
        game_map_,
        "Blinky",
        Ghost::EType::RED,
        x, y,
        Ghost::EMovingDirection::RIGHT,
        FindPathPatternBlinky);
}

std::unique_ptr<Ghost> GhostFactory::CreateGhostInky() {
    const auto [x, y] = game_map_.FromRowColToCoords(8, 7);
    return std::make_unique<Ghost>(
        texture_manager_,
        game_map_,
        "Inky",
        Ghost::EType::BLUE,
        x, y,
        Ghost::EMovingDirection::DOWN,
        FindPathPatternInky);
}

std::unique_ptr<Ghost> GhostFactory::CreateGhostPinky() {
    const auto [x, y] = game_map_.FromRowColToCoords(9, 8);
    return std::make_unique<Ghost>(
        texture_manager_,
        game_map_,
        "Pinky",
        Ghost::EType::PINK,
        x, y,
        Ghost::EMovingDirection::UP,
        FindPathPatternPinky);
}

std::unique_ptr<Ghost> GhostFactory::CreateGhostClyde() {
    const auto [x, y] = game_map_.FromRowColToCoords(8, 9);
    return std::make_unique<Ghost>(
        texture_manager_,
        game_map_,
        "Clyde",
        Ghost::EType::YELLOW,
        x, y,
        Ghost::EMovingDirection::DOWN,
        FindPathPatternClyde);
}
