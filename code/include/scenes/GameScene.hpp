#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "utils/CountdownTimer.hpp"
#include "utils/Renderer.hpp"
#include "utils/TextureManager.hpp"
#include "utils/TextManager.hpp"
#include "utils/SoundManager.hpp"

#include "UIManager.hpp"
#include "GameMap.hpp"
#include "Ghost.hpp"
#include "GhostFactory.hpp"
#include "Player.hpp"
#include "CollectableManager.hpp"
#include "Types.hpp"
#include "CollisionManager.hpp"
#include "Level.hpp"
#include "SoundPlayer.hpp"

#include "scenes/IScene.hpp"

#include "pathfinder/Pathfinder.hpp"

#include <optional>
#include <memory>
#include <string_view>
#include <string>


class GameScene : public IScene {
public:
    enum class EGameState {
        READY_TO_PLAY,  // First intro music.
        PLAYING,        // After intro music.
        ON_PLAYER_DIED, // Finished dying animation.
        GAMEOVER,       // Finished dying animation & no lifes remaining.
        ON_PLAYER_WIN,  // After collecting last collectable (executes 1 time)
        PLAYER_WON      // After ON_PLAYER_WIN executed 1 time.
    };
    
    GameScene(
        Renderer& renderer,
        SoundManager& sound_manager,
        TextureManager& texture_manager,
        TextManager& text_manager);

    void Update(float dt) override;
    void Render() override;
    void OnEvent(const SDL_Event& event, Game* game = nullptr) override;

    void StartGhostFrightenedTimer();
    float GetSecondsToFinishFrightenedMode() const;

    bool IsReadyToPlay() const;
    bool IsPlaying() const;
    bool IsGameOver() const;

    Pathfinder& GetPathfinder();
    const GameMap& GetMap() const;
    const Player& GetPlayer() const;
 
    OptionalGhostReference GetGhost(std::string_view name) const;

    void OnGhostDie(Ghost& ghost);
    void OnPlayerDie();

    void GhostInEyesStateArrivedToHouse();

    int GetFrightenedDeadsCount() const;
    void ResetFrightenedDeadsCount();
    void IncreaseFrightenedDeadsCount();

private:
    Renderer& renderer_;
    SoundManager& sound_manager_;
    TextureManager& texture_manager_;
    TextManager& text_manager_;
    
    SoundPlayer sound_player_;
    EGameState state_;
    Level level_;
    
    CountdownTimer timer_to_start_{4.5f};
    CountdownTimer timer_to_restart_{1.f};
    CountdownTimer timer_after_win_{5.5f};
    CountdownTimer key_spam_prevent_timer_{.5f};
    bool is_key_hack_able_{true};

    bool is_timer_mode_frightened_active_;
    CountdownTimer timer_mode_frightened_{0.f};
    int frightened_deads_count_ {0};

    bool is_showing_ghost_score_ {false};
    CountdownTimer timer_showing_ghost_score_ {1.5f};

    // Game Objects
    GameMap map_;
    Pathfinder pathfinder_;
    Player player_;
    GhostFactory ghost_factory_;
    GhostList ghosts_;
    CollectableManager collectable_manager_;
    CollisionManager collision_manager_;
    
    SDL_Texture* background_texture_;
    UIManager ui_manager_;
    bool did_player_win_ {false};

    void Init();
    void StartGame();
    
    void HandleStatePlaying(float dt);
    void HandleOnPlayerDied(float dt);
    void HandleStateOnPlayerWin();
    void HandleStateGameOver(float dt);
};
