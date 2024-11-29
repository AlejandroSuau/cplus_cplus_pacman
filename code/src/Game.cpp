#include "Game.hpp"

#include "Constants.hpp"
#include "Player.hpp"
#include "Ghost.hpp"
#include "utils/Collisions.hpp"

#include <ranges>
#include <stdexcept>
#include <string>
#include <algorithm>

Game::Game()
    : sdl_(std::make_unique<SDLInitializer>())
    , sdl_image_(std::make_unique<SDLImageInitializer>())
    , sdl_ttf_(std::make_unique<SDLTTFInitializer>())
    , sdl_mixer_(std::make_unique<SDLMixerInitializer>())
    , window_(
        SDL_CreateWindow(
            "Pac-Man",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            kGameWidth + (kGamePaddingX * 2),
            kGameHeight + (kGamePaddingY * 2),
            0),
        SDL_DestroyWindow)
    , renderer_(
        SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED),
        SDL_DestroyRenderer)
    , is_running_(false)
    , texture_manager_(*renderer_.get())
    , state_(EGameState::READY_TO_PLAY)
    , background_texture_(nullptr)
    , map_(kGameWidth, kGameHeight, kGamePaddingX, kGamePaddingY, kCellSize)
    , pathfinder_(map_)
    , player_(texture_manager_, map_, pathfinder_)
    , ui_manager_(text_manager_, texture_manager_, player_)
    , ghost_factory_(texture_manager_, pathfinder_, map_)
    , ghosts_{{
        ghost_factory_.CreateGhostBlinky(),
        ghost_factory_.CreateGhostInky(),
        ghost_factory_.CreateGhostPinky(),
        ghost_factory_.CreateGhostClyde()
    }}
    , collectable_manager_(texture_manager_, map_)
    , collision_manager_(player_, ghosts_, collectable_manager_) {

    if (!window_ || !renderer_) {
        throw std::runtime_error(
            std::string("Error creating the game") + SDL_GetError());
    }
    
    SDL_SetRenderDrawBlendMode(renderer_.get(), SDL_BLENDMODE_BLEND);
}

void Game::Run() {
    Init();
    
    is_running_ = true;
    SDL_ShowWindow(window_.get());

    Uint64 previous_time = SDL_GetTicks64();
    Uint64 accumulated_time = 0;
    while (is_running_) {
        Uint64 current_time = SDL_GetTicks64();
        Uint64 frame_time = current_time - previous_time; // Delta time in milliseconds
        previous_time = current_time;
        accumulated_time += frame_time;

        HandleEvents();

        // Fixed Update Loop
        while (accumulated_time >= kFixedTimeStep) {
            Update(kFixedTimeStep / 1000.0f); // Pass in seconds as a float
            accumulated_time -= kFixedTimeStep;
        }

        Render();

        Uint64 frame_end = SDL_GetTicks64();
        Uint64 frame_duration = frame_end - current_time;
        if (frame_duration < kFrameDelay) {
            SDL_Delay(static_cast<Uint32>(kFrameDelay - frame_duration));
        }
    }
}

void Game::Init() {
    background_texture_ = texture_manager_.LoadTexture(kAssetsFolderImages + "maze.png");
}

void Game::Update(float dt) {
    player_.Update(dt);
    for (auto& ghost : ghosts_) {
        ghost->Update(dt); // TODO: LO METEMOS AQUI
        if (ghost->IsInStateChasing()) {
            ghost->FindPath(*this);
        }
    }

    collision_manager_.CheckCollisions();

    collectable_manager_.RemoveCollectablesMarkedForDestroy();

    if (player_.IsDying()) {
        state_ = EGameState::GAMEOVER;
    } else if (player_.IsDead() && player_.GetLifes() > 0) {
        Reset();
    }
}

void Game::Render() {
    auto* renderer = renderer_.get();
    SDL_RenderClear(renderer);
    
    //SDL_RenderCopy(renderer, background_texture_, nullptr, &kTextureRectBackground);

    map_.Render(*renderer);
    collectable_manager_.Render(*renderer);
    player_.Render(*renderer);
    for (auto& ghost : ghosts_) {
        ghost->Render(*renderer);
    }

    ui_manager_.Render(*renderer, *this);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);
}

void Game::HandleEvents() {
     SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            is_running_ = false;
        }

        const auto is_key_down = (event.type == SDL_KEYDOWN);
        if (is_key_down) {
            player_.HandleKeyPressed(event.key.keysym.scancode);
        }
    }
}

bool Game::IsReadyToPlay() const {
    return (state_ == EGameState::READY_TO_PLAY);
}

bool Game::IsPlaying() const {
    return (state_ == EGameState::PLAYING);
}

bool Game::IsGameOver() const {
    return (state_ == EGameState::GAMEOVER);
}

void Game::HandlePressedKeySpace() {
    /*switch(state_) {
        case EGameState::FINISHED:
            Reset();
        break;
        case EGameState::READY_TO_PLAY:
            state_ = EGameState::PLAYING;
        case EGameState::PLAYING:

        break;
    }*/
}

OptionalGhostReference Game::GetGhost(std::string_view name) const {
    auto it = std::ranges::find_if(ghosts_, [&name](const auto& ghost) {
        return (ghost && ghost->GetName() == name);
    });

    return (it == ghosts_.end()) ? std::nullopt : OptionalGhostReference(**it);
}

void Game::Reset() {
    state_ = EGameState::READY_TO_PLAY;
    player_.Reset();
    for (auto& ghost : ghosts_) {
        ghost->Reset();
    }
}

Pathfinder& Game::GetPathfinder() {
    return pathfinder_;
}

const GameMap& Game::GetMap() const {
    return map_;
}

const Player& Game::GetPlayer() const { 
    return player_;
}