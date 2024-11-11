#pragma once

#include <SDL2/SDL.h>

#include "utils/Vec2.hpp"
#include "utils/TextureManager.hpp"
#include "utils/CountdownTimer.hpp"

#include "pathfinder/Pathfinder.hpp"

#include "GameMap.hpp"
#include "ScoreManager.hpp"

class Player {
public:
    Player(
        TextureManager& texture_manager,
        GameMap& game_map,
        Pathfinder& pathfinder,
        ScoreManager& score_manager,
        Vec2 position);

    void Update(float dt);
    void Render(SDL_Renderer& renderer);

    const Vec2& GetPosition() const;

    void ProcessGhostCollision();
    void ProcessBigCoinCollision();
    void ProcessCoinCollision();

    void HandleKeyPressed(SDL_Scancode scancode);

private:
    enum class EMovingDirection {
        UP, DOWN, RIGHT, LEFT
    };

    TextureManager& texture_manager_;
    GameMap& game_map_;
    Pathfinder& pathfinder_;
    ScoreManager& score_manager_;
    Vec2 position_;
    CountdownTimer moving_timer_;
    EMovingDirection moving_direction_;
    const SDL_Color color_;

    void Move();
};
