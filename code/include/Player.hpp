#pragma once

#include <SDL2/SDL.h>

#include "utils/Renderer.hpp"
#include "utils/Vec2.hpp"
#include "utils/TextureManager.hpp"
#include "utils/CountdownTimer.hpp"
#include "utils/EntityMovable.hpp"

#include "Level.hpp"

class GameMap;

class Player : public EntityMovable {
public:
    Player(
        Renderer& renderer,
        TextureManager& texture_manager,
        const GameMap& game_map,
        const Level& level);

    void Die();
    void Reset();

    // Entity
    void Update(float dt, GameScene* game_scene = nullptr) override;
    void Render() override;

    void HandleKeyPressed(const SDL_Scancode& scancode);
    
    void IncreaseScore(unsigned int value);
    void IncreaseOneLife();
    void DecreaseOneLife();
    unsigned int GetLifes() const;
    unsigned int GetScore() const;
    void Stop();

    bool IsDying() const;
    bool IsDead() const;
    bool HasLifes() const;

    void SetStateReady();

private:
    enum class EState {
        STOP,
        READY,
        MOVING,
        DYING,
        DEAD
    };

    TextureManager& texture_manager_;
    const Level& level_;
    CountdownTimer moving_timer_{0.3f};
    EDirection next_direction_;
    EState state_;
    unsigned int lifes_;
    unsigned int score_;

    SDL_Texture* sprite_sheet_;

    CountdownTimer animation_timer_moving_{0.06f};
    int sprite_index_moving_{2};

    CountdownTimer animation_timer_dying_{.2f};
    int sprite_index_dying_{0};

    void Init();

    void UpdateStateMoving(float dt);

    double GetAssetAngleOnDying() const;
    double GetAssetAngleOnMoving() const;
};
