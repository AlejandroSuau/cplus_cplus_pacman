#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "utils/CountdownTimer.hpp"
#include "utils/Vec2.hpp"
#include "utils/TextureManager.hpp"
#include "utils/EntityMovable.hpp"

#include "GhostMovementPatterns.hpp"
#include "pathfinder/Pathfinder.hpp"

#include <string>
#include <string_view>
#include <array>
#include <memory>

class Game;

class Ghost : public EntityMovable {
public:
    enum class EState {
        HOUSING,
        CHASING,
        FRIGHTENED,
        EYES,
        STOP
    };

    enum class EType {
        RED = 0,
        PINK = 1,
        BLUE = 2,
        YELLOW = 3
    };

    /*enum class EMovingDirection {
        UP = 0,
        DOWN = 1,
        LEFT = 2,
        RIGHT = 3
    };*/
    
    Ghost(
        Renderer& renderer,
        TextureManager& texture_manager,
        const GameMap& game_map,
        Pathfinder& pathfinder,
        std::string name,
        EType type,
        SDL_Rect hitbox,
        float velocity,
        EDirection direction,
        PathfindingPattern pathfinding_pattern);

    void FindPath(Game& game);

    void Reset();
    void Update(float dt) override;
    void Render() override;
    void StepPath(float dt);

    void Die();

    void SetStateStop();
    void SetStateFrightened();
    bool IsInStateEyes() const;
    bool IsInStateFrightened() const;
    bool IsInStateChasing() const;

    const std::string_view GetName() const;

private:
    TextureManager& texture_manager_;
    Pathfinder& pathfinder_;
    const std::string name_;
    EType type_;
    PathfindingPattern patfinder_pattern_;
    EState state_;

    CountdownTimer timer_mode_house_ {2.f};
    CountdownTimer timer_mode_house_swap_direction_ {.25f};
    
    const float intermittent_time_last_seconds_ = 4.f;
    CountdownTimer timer_frightened_intermittent_ {2.f};
    CountdownTimer timer_mode_frightened_ {6.f};
    int frightened_animation_index = 0;
    
    bool is_moving_between_tiles_;
    Pathfinder::Path path_;
    std::size_t path_index_;
    
    CountdownTimer animation_timer_;
    int sprite_index_;
    int sprites_count_;
    SDL_Texture* sprite_sheet_;

    void RenderPath();
    SDL_Rect GetSourceRect() const;

    void UpdateStateHouse(float dt);
    void UpdateStateChasing(float dt);
    void UpdateStateFrightened(float dt);
    void UpdateStateEyes(float dt);

    void SetStateEyes(Game& game);
    void SetHousingState();
};
