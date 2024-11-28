#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "utils/CountdownTimer.hpp"
#include "utils/Vec2.hpp"
#include "utils/TextureManager.hpp"

#include "GameMap.hpp"
#include "GhostMovementPatterns.hpp"

#include <string>
#include <string_view>

class Game;

class Ghost {
public:
    enum class EState {
        HOUSING,
        CHASING,
        FRIGHTENED,
        EYES,
        DEAD
    };

    enum class EType {
        RED = 0,
        PINK = 1,
        BLUE = 2,
        YELLOW = 3
    };

    enum class EMovingDirection {
        UP = 0,
        DOWN = 1,
        LEFT = 2,
        RIGHT = 3
    };
    
    Ghost(
        TextureManager& texture_manager,
        const GameMap& game_map,
        std::string name,
        EType type,
        int x,
        int y,
        EMovingDirection direction,
        PathfindingPattern pathfinding_pattern);

    void FindPath(Game& game);

    void Update(float dt);
    void Render(SDL_Renderer& renderer);

    void OnCollisionWithPlayer(Game& game);

    void ActivateFrightenedMode();

    Vec2 GetPosition() const;
    const std::string_view GetName() const;
    Vec2 GetDirectionVector(EMovingDirection direction) const;
    Vec2 GetDirectionVector() const;
    bool IsOnChasingState() const;
    const SDL_Rect& GetHibox() const;

private:
    TextureManager& texture_manager_;
    const GameMap& game_map_;
    const std::string name_;
    EType type_;
    SDL_Rect hitbox_;
    const Vec2 starting_position_;
    EMovingDirection direction_;
    const EMovingDirection starting_direction_;
    PathfindingPattern patfinder_pattern_;
    EState state_;

    CountdownTimer timer_mode_house_ {2.f};
    CountdownTimer timer_mode_house_swap_direction_{.25f};
    CountdownTimer timer_mode_frightened_ {8.f};
    
    bool is_moving_between_tiles_;
    Pathfinder::Path path_;
    std::size_t path_index_;
    
    CountdownTimer animation_timer_;
    int sprite_index_;
    int sprites_count_;
    SDL_Texture* sprite_sheet_;

    void RenderPath(SDL_Renderer& renderer);
    SDL_Rect GetSourceRect() const;

    void ReverseDirection();
    EMovingDirection GetOppositeDirection() const;

    bool TryToMove(Vec2 delta_movement);
    bool IsMovementAllowed(SDL_Rect moved_rect) const;

    void UpdateStateHouse(float dt);
    void UpdateStateChasing(float dt);
    void UpdateStateFrightened(float dt);
    void UpdateStateEyes(float dt);

    void SetStateEyes(Game& game);
    void SetHousingState();
};
