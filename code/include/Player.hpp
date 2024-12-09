#pragma once

#include <SDL2/SDL.h>

#include "utils/Renderer.hpp"
#include "utils/Vec2.hpp"
#include "utils/TextureManager.hpp"
#include "utils/CountdownTimer.hpp"
#include "utils/EntityMovable.hpp"

class GameMap;

class Player : public EntityMovable {
public:
    Player(
        Renderer& renderer,
        TextureManager& texture_manager,
        const GameMap& game_map);

    void Die();
    void Reset();

    // Entity
    void Update(float dt) override;
    void Render() override;

    void HandleKeyPressed(const SDL_Scancode& scancode);
    
    void IncreaseScore(unsigned int value);
    void IncreaseOneLife();
    void DecreaseOneLife();
    unsigned int GetLifes() const;
    unsigned int GetScore() const;

    bool IsDying() const;
    bool IsDead() const;
    bool IsMoving() const;

private:
    enum class EState {
        READY,
        MOVING,
        DYING,
        DEAD
    };

    TextureManager& texture_manager_;
    CountdownTimer moving_timer_{0.3f};
    EDirection next_direction_;
    EState state_;
    unsigned int lifes_;
    unsigned int score_;

    SDL_Texture* sprite_sheet_;
    CountdownTimer moving_animation_timer_{0.1f};
    int moving_animation_sprite_index_{0};

    CountdownTimer dying_animation_timer_{0.08f};
    int dying_animation_sprite_index_{0};

    SDL_Rect GetSourceRectMoving() const;
    SDL_Rect GetSourceRectDying() const;
    void UpdateStateMoving(float dt);
};
