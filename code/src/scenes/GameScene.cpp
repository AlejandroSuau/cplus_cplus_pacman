#include "scenes/GameScene.hpp"

#include "Constants.hpp"
#include "Player.hpp"
#include "Ghost.hpp"

#include "scenes/MainMenuScene.hpp"

#include "utils/Collisions.hpp"

#include <ranges>
#include <stdexcept>
#include <string>
#include <algorithm>

GameScene::GameScene(
    Renderer& renderer,
    SoundManager& sound_manager,
    TextureManager& texture_manager,
    TextManager& text_manager)
    : renderer_(renderer)
    , sound_manager_(sound_manager)
    , texture_manager_(texture_manager)
    , text_manager_(text_manager)
    , ui_manager_(renderer, text_manager_, texture_manager_, player_, level_)
    , state_(EGameState::READY_TO_PLAY)
    , map_(
        renderer_,
        kGameWidth,
        kGameHeight,
        Vec2{static_cast<float>(kGamePaddingX), static_cast<float>(kGamePaddingY)},
        kCellSize)
    , pathfinder_(map_)
    , player_(renderer_, texture_manager_, map_, level_)
    , ghost_factory_(renderer_, texture_manager_, map_, pathfinder_, level_)
    , ghosts_{{
        ghost_factory_.CreateGhostBlinky(),
        ghost_factory_.CreateGhostInky(),
        ghost_factory_.CreateGhostPinky(),
        ghost_factory_.CreateGhostClyde()
    }}
    , collectable_manager_(renderer_, texture_manager_, map_)
    , collision_manager_(player_, ghosts_, collectable_manager_)
    , did_player_win_(false) {
    Init();
}

void GameScene::Init() {
    timer_to_start_.SetOnFinishCallback([this]() {
        state_ = EGameState::PLAYING;
        player_.SetStateReady();
    });
    key_spam_prevent_timer_.SetOnFinishCallback([this]() { is_key_hack_able_ = true; });
    timer_to_restart_.SetOnFinishCallback([this]() { Reset();});
}

void GameScene::Reset() {
    if (did_player_win_) {
        collectable_manager_.CreateCollectables();
    }

    timer_to_start_.Restart();
    timer_to_restart_.Restart();
    did_player_win_ = false;
    player_.Reset();
    for (auto& ghost : ghosts_) { ghost->Reset(); }
    
    state_ = EGameState::READY_TO_PLAY;  
}

void GameScene::Update(float dt) {
    if (!is_key_hack_able_) { key_spam_prevent_timer_.Update(dt); }

    player_.Update(dt);
    for (auto& ghost : ghosts_) {
        ghost->Update(dt, this);
    }

    switch(state_) {
        case EGameState::READY_TO_PLAY: timer_to_start_.Update(dt);    break;
        case EGameState::PLAYING:       HandleStatePlaying(dt);        break;
        case EGameState::ON_PLAYER_DIE: state_ = EGameState::GAMEOVER; break;
        case EGameState::GAMEOVER:      HandleStateGameOver(dt);       break;
        case EGameState::ON_PLAYER_WIN: HandleStateOnPlayerWin();      break;
        case EGameState::PLAYER_WON:    timer_to_restart_.Update(dt);  break;
    }
}

void GameScene::HandleStatePlaying(float dt) {
    collision_manager_.CheckCollisions();
    collectable_manager_.RemoveCollectablesMarkedForDestroy();
    if (collectable_manager_.DidCollectAll()) {
        state_ = EGameState::ON_PLAYER_WIN;
    } else if (player_.IsDying()) {
        state_ = EGameState::ON_PLAYER_DIE;
    }
}

void GameScene::HandleStateOnPlayerWin() {
    did_player_win_ = true;
    for (auto& ghost : ghosts_) ghost->SetStateStop();
    player_.Stop();
    level_.IncreaseLevel();
    state_ = EGameState::PLAYER_WON;
}

void GameScene::HandleStateGameOver(float dt) {
    if (player_.HasLifes()) {
        timer_to_restart_.Update(dt);
    } else {
        // TODO: Go back to menu i.e.
    }
}

void GameScene::OnEvent(const SDL_Event& event, Game* game) {
    if (event.type != SDL_KEYDOWN) return;

    player_.HandleKeyPressed(event.key.keysym.scancode);
    
    if (!is_key_hack_able_) return;
    switch(event.key.keysym.scancode) {
        case SDL_SCANCODE_F:
            for (auto& g : ghosts_) g->SetStateFrightened();
            is_key_hack_able_ = false;
        break;
        case SDL_SCANCODE_D:
            for (auto& g : ghosts_) g->Die();
            is_key_hack_able_ = false;
        break;
        case SDL_SCANCODE_L:
            player_.IncreaseOneLife();
            is_key_hack_able_ = false;
        break;
        case SDL_SCANCODE_C:
            player_.IncreaseScore(collectable_manager_.GetAllCollectableScores());
            collectable_manager_.MarkAllForDestroy();
            is_key_hack_able_ = false;
        break;
    }
}

void GameScene::Render() {   
    map_.Render();
    collectable_manager_.Render();
    for (auto& ghost : ghosts_) {
        ghost->Render();
    }
    player_.Render();
    ui_manager_.Render(*this);
}

bool GameScene::IsReadyToPlay() const {
    return (state_ == EGameState::READY_TO_PLAY);
}

bool GameScene::IsPlaying() const {
    return (state_ == EGameState::PLAYING);
}

bool GameScene::IsGameOver() const {
    return (state_ == EGameState::GAMEOVER);
}

OptionalGhostReference GameScene::GetGhost(std::string_view name) const {
    auto it = std::ranges::find_if(ghosts_, [&name](const auto& ghost) {
        return (ghost && ghost->GetName() == name);
    });

    return (it == ghosts_.end()) ? std::nullopt : OptionalGhostReference(**it);
}

Pathfinder& GameScene::GetPathfinder() {
    return pathfinder_;
}

const GameMap& GameScene::GetMap() const {
    return map_;
}

const Player& GameScene::GetPlayer() const { 
    return player_;
}