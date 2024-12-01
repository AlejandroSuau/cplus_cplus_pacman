#pragma once

#include "utils/SDLInitializer.hpp"
#include "utils/SDLImageInitializer.hpp"
#include "utils/SDLTTFInitializer.hpp"
#include "utils/SDLMixerInitializer.hpp"
#include "utils/CountdownTimer.hpp"
#include "utils/Renderer.hpp"
#include "utils/TextureManager.hpp"
#include "utils/SoundsManager.hpp"

#include "UIManager.hpp"
#include "GameMap.hpp"
#include "Ghost.hpp"
#include "GhostFactory.hpp"
#include "Player.hpp"
#include "CollectableManager.hpp"
#include "Types.hpp"
#include "CollisionManager.hpp"

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
        GAMEOVER
    };

    Game();
    void Run();

    bool IsReadyToPlay() const;
    bool IsPlaying() const;
    bool IsGameOver() const;

    Pathfinder& GetPathfinder();
    const GameMap& GetMap() const;
    const Player& GetPlayer() const;
 
    OptionalGhostReference GetGhost(std::string_view name) const;

private:
    // SDL Initializers
    std::unique_ptr<SDLInitializer> sdl_;
    std::unique_ptr<SDLImageInitializer> sdl_image_;
    std::unique_ptr<SDLTTFInitializer> sdl_ttf_;
    std::unique_ptr<SDLMixerInitializer> sdl_mixer_;

    // SDL window & render
    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window_;
    std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> sdl_renderer_;
    bool is_running_;

    Renderer renderer_;
    SoundsManager sounds_manager_;
    TextureManager texture_manager_;
    TextManager text_manager_;
    UIManager ui_manager_;
    EGameState state_;
    
    CountdownTimer key_spam_prevent_timer_{.5f};
    bool is_key_hack_able_{true};

    // Game Objects
    GameMap map_;
    Pathfinder pathfinder_;
    Player player_;
    GhostFactory ghost_factory_;
    GhostList ghosts_;
    CollectableManager collectable_manager_;
    CollisionManager collision_manager_;
 
    void Init();

    void Update(float dt);
    void Render();
    void HandleEvents();
    void HandlePressedKeySpace();

    void Reset();
};
