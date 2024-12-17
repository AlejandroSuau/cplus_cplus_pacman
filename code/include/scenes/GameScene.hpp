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
#include "Constants.hpp"

#include "scenes/IScene.hpp"

#include "pathfinder/Pathfinder.hpp"

#include <optional>
#include <memory>
#include <string_view>
#include <string>

class BackgroundMusicPlayer {
public:
    BackgroundMusicPlayer(SoundManager& sound_manager)
        : sound_manager_(sound_manager) { LoadMusic(); }
    
    void PlayMusicIntro()      { PlayMusic(0, false); }
    void PlayMusicPlaying()    { PlayMusic(1, true);  }
    void PlayMusicEyes()       { PlayMusic(2, true);  }
    void PlayMusicFrightened() { PlayMusic(3, true);  }
    void PlayMusicWin()        { PlayMusic(4, true);  }

    void StopMusic()           { Mix_HaltChannel(-1); }

private:
    SoundManager& sound_manager_;
    std::size_t last_playing_sound_index_ {0};

    static const std::size_t kAvailableSongs = 5;
    std::array<Mix_Chunk*, kAvailableSongs> songs_{};

    void LoadMusic() {
        static const std::array<std::string, kAvailableSongs> song_names_ {
            "start.wav", "siren0_firstloop.wav", "eyes_firstloop.wav", "fright_firstloop.wav", "intermission.wav"};
        for (std::size_t i = 0; i < kAvailableSongs; ++i) {
            songs_[i] = sound_manager_.LoadSoundEffect(kAssetsFolderSounds + song_names_[i]);
        }
    }

    void PlayMusic(std::size_t index, bool loop = true) {
        StopMusic();
        Mix_PlayChannel(-1, songs_[index], loop ? -1 : 0);
    }
};

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

    int GetFrightenedDeadsCount() const;
    void ResetFrightenedDeadsCount();
    void IncreaseFrightenedDeadsCount();

private:
    Renderer& renderer_;
    SoundManager& sound_manager_;
    TextureManager& texture_manager_;
    TextManager& text_manager_;
    
    BackgroundMusicPlayer music_player_;
    EGameState state_;
    Level level_;
    
    CountdownTimer timer_to_start_{4.5f};
    CountdownTimer timer_to_restart_{1.f};
    CountdownTimer key_spam_prevent_timer_{.5f};
    bool is_key_hack_able_{true};

    bool is_timer_mode_frightened_active_;
    CountdownTimer timer_mode_frightened_{0.f};
    int frightened_deads_count_ {0};

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
