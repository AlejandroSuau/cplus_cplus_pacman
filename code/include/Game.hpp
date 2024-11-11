#pragma once

#include "utils/SDLInitializer.hpp"
#include "utils/SDLImageInitializer.hpp"
#include "utils/SDLTTFInitializer.hpp"
#include "utils/SDLMixerInitializer.hpp"
#include "utils/CountdownTimer.hpp"

#include "utils/TextureManager.hpp"
#include "utils/SoundsManager.hpp"

#include "UIManager.hpp"
#include "ScoreManager.hpp"
#include "GameMap.hpp"
#include "Ghost.hpp"
#include "Player.hpp"

#include "pathfinder/Pathfinder.hpp"

#include <memory>
#include <vector>
#include <array>

class Game {
public:
    enum class EGameState {
        READY_TO_PLAY,
        PLAYING,
        FINISHING,
        FINISHED
    };

    Game();
    void Run();

    bool IsReadyToPlay() const;
    bool IsPlaying() const;
    bool IsFinishing() const;
    bool IsFinished() const;

private:
    // SDL Initializers
    std::unique_ptr<SDLInitializer> sdl_;
    std::unique_ptr<SDLImageInitializer> sdl_image_;
    std::unique_ptr<SDLTTFInitializer> sdl_ttf_;
    std::unique_ptr<SDLMixerInitializer> sdl_mixer_;

    // SDL window & render
    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window_;
    std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> renderer_;
    bool is_running_;

    SoundsManager sounds_manager_;
    TextureManager texture_manager_;
    ScoreManager score_manager_;
    UIManager ui_manager_;
    EGameState state_;
    SDL_Texture* background_texture_;

    // Game Objects
    GameMap map_;
    Pathfinder pathfinder_;
    Pathfinder::Path path_;
    Player player_;
    std::array<Ghost, 4> ghosts_;
    CountdownTimer search_countdown_{1000};
    // 4 Ghosts
    // Collectables
    // Big collectables
    // Player

    void Init();

    void Update(float dt);
    void Render();
    void HandleEvents();
    void HandlePressedKeySpace();

    void Reset();
};
