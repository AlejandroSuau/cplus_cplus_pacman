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
    , player_(renderer_, texture_manager_, map_)
    , ghost_factory_(renderer_, texture_manager_, map_, pathfinder_)
    , ghosts_{{
        ghost_factory_.CreateGhostBlinky(),
        ghost_factory_.CreateGhostInky(),
        ghost_factory_.CreateGhostPinky(),
        ghost_factory_.CreateGhostClyde()
    }}
    , collectable_manager_(renderer_, texture_manager_, map_)
    , collision_manager_(player_, ghosts_, collectable_manager_) {}

void GameScene::Init() {}

void GameScene::Update(float dt) {
    if (state_ == EGameState::READY_TO_PLAY) {
        timer_to_start_.Update(dt);
        if (timer_to_start_.DidFinish()) {
            state_ = EGameState::PLAYING;
        }
    }

    if (!is_key_hack_able_) {
        key_spam_prevent_timer_.Update(dt);
        if (key_spam_prevent_timer_.DidFinish()) is_key_hack_able_ = true;
    }

    player_.Update(dt);
    for (auto& ghost : ghosts_) {
        ghost->Update(dt);
        if (ghost->IsInStateChasing()) {
            ghost->FindPath(*this);
        }
    }

    collision_manager_.CheckCollisions();

    collectable_manager_.RemoveCollectablesMarkedForDestroy();

    if (player_.IsDying()) {
        state_ = EGameState::GAMEOVER;
    } else if (player_.IsDead() && player_.GetLifes() > 0) {
        timer_to_restart_.Update(dt);
        if (timer_to_restart_.DidFinish()) Reset();
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

void GameScene::OnEventKeyboard(EEventKeyboard event, SDL_Scancode scancode, Game& /*game*/) {
    if (event != EEventKeyboard::DOWN) return;

    player_.HandleKeyPressed(scancode);
    
    // TODO: Create a class for handling commands.
    if (!is_key_hack_able_) return;
    switch(scancode) {
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
    }
}

void GameScene::OnEventMouse(EEventMouse event, const Vec2<float> coords, Game& game) {

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

void GameScene::Reset() {
    state_ = EGameState::READY_TO_PLAY;
    player_.Reset();
    for (auto& ghost : ghosts_) {
        ghost->Reset();
    }
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