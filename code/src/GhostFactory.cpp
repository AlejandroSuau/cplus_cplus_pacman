#include "GhostFactory.hpp"

#include "Game.hpp"
#include "GhostMovementPatterns.hpp"

GhostFactory::GhostFactory(
    Renderer& renderer,
    TextureManager& texture_manager,
    const GameMap& game_map, 
    Pathfinder& pathfinder)
    : renderer_(renderer)
    , texture_manager_(texture_manager)
    , pathfinder_(pathfinder)
    , game_map_(game_map) {}

std::unique_ptr<Ghost> GhostFactory::CreateGhostBlinky() {
    const auto [x, y] = game_map_.FromRowColToCoords(6, 8);
    return std::make_unique<Ghost>(
        renderer_,
        texture_manager_,
        game_map_,
        pathfinder_,
        "Blinky",
        Ghost::EType::RED,
        SDL_Rect{x, y, 31, 31},
        150.f,
        EDirection::RIGHT,
        FindPathPatternBlinky);
}

std::unique_ptr<Ghost> GhostFactory::CreateGhostInky() {
    const auto [x, y] = game_map_.FromRowColToCoords(8, 7);
    return std::make_unique<Ghost>(
        renderer_,
        texture_manager_,
        game_map_,
        pathfinder_,
        "Inky",
        Ghost::EType::BLUE,
        SDL_Rect{x, y, 31, 31},
        150.f,
        EDirection::DOWN,
        FindPathPatternInky);
}

std::unique_ptr<Ghost> GhostFactory::CreateGhostPinky() {
    const auto [x, y] = game_map_.FromRowColToCoords(9, 8);
    return std::make_unique<Ghost>(
        renderer_,
        texture_manager_,
        game_map_,
        pathfinder_,
        "Pinky",
        Ghost::EType::PINK,
        SDL_Rect{x, y, 31, 31},
        150.f,
        EDirection::UP,
        FindPathPatternPinky);
}

std::unique_ptr<Ghost> GhostFactory::CreateGhostClyde() {
    const auto [x, y] = game_map_.FromRowColToCoords(8, 9);
    return std::make_unique<Ghost>(
        renderer_,
        texture_manager_,
        game_map_,
        pathfinder_,
        "Clyde",
        Ghost::EType::YELLOW,
        SDL_Rect{x, y, 31, 31},
        150.f,
        EDirection::DOWN,
        FindPathPatternClyde);
}
