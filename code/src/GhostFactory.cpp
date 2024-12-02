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
    const auto coords = game_map_.FromColRowToCoords(Vec2<int>{8, 6});
    return std::make_unique<Ghost>(
        renderer_,
        texture_manager_,
        game_map_,
        pathfinder_,
        "Blinky",
        Ghost::EType::RED,
        SDL_FRect{coords.x, coords.y, 30.f, 30.f},
        110.f,
        EDirection::RIGHT,
        FindPathPatternBlinky);
}

std::unique_ptr<Ghost> GhostFactory::CreateGhostInky() {
    const auto coords = game_map_.FromColRowToCoords(Vec2<int>{7, 8});
    return std::make_unique<Ghost>(
        renderer_,
        texture_manager_,
        game_map_,
        pathfinder_,
        "Inky",
        Ghost::EType::BLUE,
        SDL_FRect{coords.x, coords.y, 30.f, 30.f},
        120.f,
        EDirection::DOWN,
        FindPathPatternInky);
}

std::unique_ptr<Ghost> GhostFactory::CreateGhostPinky() {
    const auto coords = game_map_.FromColRowToCoords(Vec2<int>{8, 9});
    return std::make_unique<Ghost>(
        renderer_,
        texture_manager_,
        game_map_,
        pathfinder_,
        "Pinky",
        Ghost::EType::PINK,
        SDL_FRect{coords.x, coords.y, 30.f, 30.f},
        100.f,
        EDirection::UP,
        FindPathPatternPinky);
}

std::unique_ptr<Ghost> GhostFactory::CreateGhostClyde() {
    const auto coords = game_map_.FromColRowToCoords(Vec2<int>{9, 8});
    return std::make_unique<Ghost>(
        renderer_,
        texture_manager_,
        game_map_,
        pathfinder_,
        "Clyde",
        Ghost::EType::YELLOW,
        SDL_FRect{coords.x, coords.y, 30.f, 30.f},
        140.f,
        EDirection::DOWN,
        FindPathPatternClyde);
}
