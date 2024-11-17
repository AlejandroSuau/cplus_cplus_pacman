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
        const GameMap& game_map,
        Pathfinder& pathfinder,
        ScoreManager& score_manager);

    void Update(float dt);
    void Render(SDL_Renderer& renderer);

    void ProcessGhostCollision();
    void ProcessBigCoinCollision();
    void ProcessCoinCollision();

    void HandleKeyPressed(SDL_Scancode scancode);

    void DecreaseOneLife();
    unsigned int GetLifes() const;
    const SDL_Rect& GetHitbox() const;

    Vec2 GetPosition() const;

private:
    enum class EMovingDirection {
        LEFT = 0,
        UP = 1,
        RIGHT = 2,
        DOWN = 3
    };

    enum class EStatus {
        READY,
        MOVING,
        DYING,
        DEAD
    };

    TextureManager& texture_manager_;
    const GameMap& game_map_;
    Pathfinder& pathfinder_;
    ScoreManager& score_manager_;
    SDL_Rect hitbox_;
    CountdownTimer moving_timer_{0.3f};
    EMovingDirection direction_;
    EMovingDirection next_direction_;
    EStatus status_;
    unsigned int lifes_;

    SDL_Texture* sprite_sheet_;
    CountdownTimer moving_animation_timer_{0.1f};
    int moving_animation_sprite_index_{0};

    CountdownTimer dying_animation_timer_{0.15f};
    int dying_animation_sprite_index_{0};

    void Move(float dt);
    bool TryToMove(EMovingDirection direction, int dx, int dy);
    bool IsMovementAllowed(SDL_Rect moved_rect) const;

    SDL_Rect GetSourceRectMoving() const;
    SDL_Rect GetSourceRectDying() const;
};
