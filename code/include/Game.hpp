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
#include "GhostFactory.hpp"
#include "Player.hpp"
#include "CollectableList.hpp"

#include "pathfinder/Pathfinder.hpp"

#include <optional>
#include <memory>
#include <array>
#include <string_view>

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

    Pathfinder& GetPathfinder();
    const GameMap& GetMap() const;
    const Player& GetPlayer() const;
 
    using OptionalGhostReference = std::optional<std::reference_wrapper<const Ghost>>;
    OptionalGhostReference GetGhost(std::string_view name) const;

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
    TextManager text_manager_;
    ScoreManager score_manager_;
    UIManager ui_manager_;
    EGameState state_;
    SDL_Texture* background_texture_;

    // Game Objects
    GameMap map_;
    Pathfinder pathfinder_;
    Player player_;
    GhostFactory ghost_factory_;
    std::array<std::unique_ptr<Ghost>, 4> ghosts_;
    CountdownTimer search_countdown_{.25f};
    CollectableList collectables_;
 
    void Init();

    void Update(float dt);
    void Render();
    void HandleEvents();
    void HandlePressedKeySpace();

    void Reset();
};
