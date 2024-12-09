#pragma once

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

#include "scenes/IScene.hpp"

#include "pathfinder/Pathfinder.hpp"

#include <optional>
#include <memory>
#include <string_view>

class GameScene : public IScene {
public:
    enum class EGameState {
        READY_TO_PLAY,
        PLAYING,
        GAMEOVER
    };
    
    GameScene(
        Renderer& renderer,
        SoundManager& sound_manager,
        TextureManager& texture_manager,
        TextManager& text_manager);

    void Update(float dt) override;
    void Render() override;
    void OnEvent(const SDL_Event& event, Game* game = nullptr) override;

    bool IsReadyToPlay() const;
    bool IsPlaying() const;
    bool IsGameOver() const;

    Pathfinder& GetPathfinder();
    const GameMap& GetMap() const;
    const Player& GetPlayer() const;
 
    OptionalGhostReference GetGhost(std::string_view name) const;

private:
    Renderer& renderer_;
    SoundManager& sound_manager_;
    TextureManager& texture_manager_;
    TextManager& text_manager_;
    
    EGameState state_;
    Level level_;
    
    CountdownTimer timer_to_start_{2.f};
    CountdownTimer timer_to_restart_{1.f};
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
    
    UIManager ui_manager_;
 
    void Init();
    void Reset();
};
