#include "scenes/GameScene.hpp"

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
    , sound_player_(sound_manager)
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
    , collision_manager_(sound_manager_, player_, ghosts_, collectable_manager_)
    , ui_manager_(renderer, text_manager_, texture_manager_, player_, level_) {
    Init();
}

void GameScene::Init() {
    background_texture_ = texture_manager_.LoadTexture(kAssetsFolderImages + "background.png");
    timer_to_start_.SetOnFinishCallback([this]() {
        sound_player_.PlayMusicPlaying();
        state_ = EGameState::PLAYING;
        player_.SetStateReady();
        for (auto& ghost : ghosts_) { ghost->SetStateHousing(); }
    });
    key_spam_prevent_timer_.SetOnFinishCallback([this]() { is_key_hack_able_ = true; });
    timer_to_restart_.SetOnFinishCallback([this]() { StartGame();});
    timer_after_win_.SetOnFinishCallback([this]() { StartGame(); });

    timer_showing_ghost_score_.SetOnFinishCallback([this]() {
        is_showing_ghost_score_ = false;
        sound_player_.PlayMusicEyes();
        auto is_showing_score = [this](const auto& g) { return g->IsInStateShowingScore(); };
        for (auto& g : ghosts_ | std::views::filter(is_showing_score)) {
            g->SetStateEyes();
        }
    });

    timer_mode_frightened_.SetOnFinishCallback([this]() {
        auto is_frightened = [this](const auto& g) { return g->IsInStateFrightened(); };
        for (auto& g : ghosts_ | std::views::filter(is_frightened)) {
            g->SetStateChasing();
        }
        is_timer_mode_frightened_active_ = false;
        frightened_deads_count_ = 0;
        sound_player_.StopMusic();
        
        const auto is_any_ghost_in_eyes_state = std::any_of(
            ghosts_.cbegin(), ghosts_.cend(), [](const auto& g) { return g->IsInStateEyes(); });
        if (is_any_ghost_in_eyes_state) {
            sound_player_.PlayMusicEyes();
        } else {
            sound_player_.PlayMusicPlaying();
        }
    });

    StartGame();
}

void GameScene::StartGame() {
    if (did_player_win_) {
        collectable_manager_.CreateCollectables();
    }

    timer_to_start_.Restart();
    timer_to_restart_.Restart();
    did_player_win_ = false;
    player_.Reset();
    for (auto& ghost : ghosts_) { ghost->Reset(); }
    
    timer_mode_frightened_.SetIntervalSeconds(level_.GetSecondsDurationGhostFrightened());
    is_timer_mode_frightened_active_ = false;
    
    state_ = EGameState::READY_TO_PLAY;
    sound_player_.PlayMusicIntro();    
}

void GameScene::Update(float dt) {
    if (!is_key_hack_able_) { key_spam_prevent_timer_.Update(dt); }

    if (is_showing_ghost_score_) {
        timer_showing_ghost_score_.Update(dt);
        return;
    }

    player_.Update(dt);
    for (auto& ghost : ghosts_) {
        ghost->Update(dt, this);
    }

    switch(state_) {
        case EGameState::READY_TO_PLAY:  timer_to_start_.Update(dt);    break;
        case EGameState::PLAYING:        HandleStatePlaying(dt);        break;
        case EGameState::ON_PLAYER_DIED: HandleOnPlayerDied(dt);        break;
        case EGameState::GAMEOVER:       HandleStateGameOver(dt);       break;
        case EGameState::ON_PLAYER_WIN:  HandleStateOnPlayerWin();      break;
        case EGameState::PLAYER_WON:     timer_after_win_.Update(dt);  break;
    }
}

void GameScene::HandleStatePlaying(float dt) {
    if (is_timer_mode_frightened_active_) {
        timer_mode_frightened_.Update(dt);
    }

    collision_manager_.CheckCollisions(*this);
    collectable_manager_.RemoveCollectablesMarkedForDestroy();
    if (collectable_manager_.DidCollectAll()) {
        state_ = EGameState::ON_PLAYER_WIN;
    } else if (player_.IsDead()) {
        state_ = EGameState::ON_PLAYER_DIED;
    }
}

void GameScene::HandleStateOnPlayerWin() {
    did_player_win_ = true;
    for (auto& ghost : ghosts_) ghost->SetStateStop();
    player_.Stop();
    level_.IncreaseLevel();
    state_ = EGameState::PLAYER_WON;
    sound_player_.PlayMusicWin();
}

void GameScene::HandleOnPlayerDied(float dt) {
    if (player_.HasLifes()) {
        timer_to_restart_.Update(dt);
    } else {
        state_ = EGameState::GAMEOVER;
    }
}

void GameScene::HandleStateGameOver(float dt) {
    // TODO: Go back to menu i.e.
}

void GameScene::OnEvent(const SDL_Event& event, Game* game) {
    if (event.type != SDL_KEYDOWN) return;

    player_.HandleKeyPressed(event.key.keysym.scancode);
    
    if (!is_key_hack_able_) return;
    switch(event.key.keysym.scancode) {
        case SDL_SCANCODE_F:
            StartGhostFrightenedTimer();
            is_key_hack_able_ = false;
        break;
        case SDL_SCANCODE_D:
            frightened_deads_count_ = 0;
            for (auto& g : ghosts_) {
                const auto ghost_score = g->Die(++frightened_deads_count_);
                player_.IncreaseScore(ghost_score);
            }
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

void GameScene::GhostInEyesStateArrivedToHouse() {
    const auto is_any_ghost_in_eyes_state = std::any_of(
        ghosts_.cbegin(), ghosts_.cend(), [](const auto& g) { return g->IsInStateEyes(); });
    if (!is_any_ghost_in_eyes_state && !is_timer_mode_frightened_active_) {
        sound_player_.PlayMusicPlaying();
    } else if (is_timer_mode_frightened_active_) {
        sound_player_.PlayMusicFrightened();
    }
}

void GameScene::OnGhostDie(Ghost& ghost) {
    sound_player_.StopMusic();
    sound_player_.PlaySoundDieGhost();

    timer_showing_ghost_score_.Restart();
    is_showing_ghost_score_ = true;
    ++frightened_deads_count_;
    const auto ghost_score = ghost.Die(frightened_deads_count_);
    player_.IncreaseScore(ghost_score);
}

void GameScene::Render() {
    renderer_.RenderTexture(background_texture_, {0, 0, 561, 659}, {kGamePaddingX - 10.f, kGamePaddingY - 10.f, 561.f, 659.f});

    collectable_manager_.Render();
    for (auto& ghost : ghosts_) {
        ghost->Render();
    }
    player_.Render();
    ui_manager_.Render(*this);
}

void GameScene::StartGhostFrightenedTimer() {
    is_timer_mode_frightened_active_ = true;
    auto is_not_in_eyes_state = [](const auto& g) { return !g->IsInStateEyes(); };
    for (auto& ghost : ghosts_ | std::views::filter(is_not_in_eyes_state)) {
        ghost->SetStateFrightened();
    }
    timer_mode_frightened_.Restart();
    sound_player_.PlayMusicFrightened();
}

void GameScene::OnPlayerDie() {
    sound_player_.StopMusic();
    sound_player_.PlaySoundDiePlayer();
    player_.Die();
    for (auto& ghost : ghosts_) {
        ghost->SetStateStop();
    }
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

void GameScene::IncreaseFrightenedDeadsCount() {
    ++frightened_deads_count_;
}

int GameScene::GetFrightenedDeadsCount() const {
    return frightened_deads_count_;
}

void GameScene::ResetFrightenedDeadsCount() {
    frightened_deads_count_ = 0;
}

float GameScene::GetSecondsToFinishFrightenedMode() const {
    return timer_mode_frightened_.GetSecondsToFinish();
}