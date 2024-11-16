#include "Game.hpp"

#include "Constants.hpp"
#include "Player.hpp"
#include "Ghost.hpp"

#include <ranges>
#include <stdexcept>
#include <string>
#include <iostream>

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
    , score_manager_(texture_manager_)
    , ui_manager_(text_manager_, texture_manager_, score_manager_)
    , state_(EGameState::READY_TO_PLAY)
    , background_texture_(nullptr)
    , map_(kGameWidth, kGameHeight, kGamePaddingX, kGamePaddingY, kCellSize)
    , pathfinder_(map_)
    , player_(texture_manager_, map_, pathfinder_, score_manager_)
    , ghosts_{{
        {texture_manager_, map_, pathfinder_, Vec2{8 * kCellSize, 6 * kCellSize}, SDL_Color{255, 0, 0, 255}},
        {texture_manager_, map_, pathfinder_, Vec2{7 * kCellSize, 8 * kCellSize}, SDL_Color{0, 255, 0, 255}},
        {texture_manager_, map_, pathfinder_, Vec2{8 * kCellSizeInt, 8 * kCellSize}, SDL_Color{0, 0, 255, 255}},
        {texture_manager_, map_, pathfinder_, Vec2{9 * kCellSize, 8 * kCellSize}, SDL_Color{100, 100, 90, 255}}
    }}
    , collectables_(texture_manager_, score_manager_, map_) {

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
    search_countdown_.Update(dt);
    if (search_countdown_.DidFinish()) {
        /*const auto player_pos = player_.GetPosition();
        const auto [row, col] = map_.FromCoordsToRowCol(
            static_cast<int>(player_pos.x),
            static_cast<int>(player_pos.y));
        ghosts_[0].FindPath(row, col);
        for (auto& ghost : ghosts_) {
            ghost.FindPath(row, col);
        }*/
    }

    player_.Update(dt);
    for (auto& ghost : ghosts_) {
        ghost.Update(dt);
    }

    collectables_.ProcessCollisions(*this);

    collectables_.RemoveCollectablesMarkedForDestroy();
}

void Game::Render() {
    auto* renderer = renderer_.get();
    SDL_RenderClear(renderer);
    
    //SDL_RenderCopy(renderer, background_texture_, nullptr, &kTextureRectBackground);

    map_.Render(*renderer);
    player_.Render(*renderer);
    for (auto& g : ghosts_) {
        g.Render(*renderer);
    }

    collectables_.Render(*renderer);
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

bool Game::IsFinishing() const {
    return (state_ == EGameState::FINISHING);
}

bool Game::IsFinished() const {
    return (state_ == EGameState::FINISHED);
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

void Game::Reset() {
    state_ = EGameState::READY_TO_PLAY;
    score_manager_.Reset();
}

const Player& Game::GetPlayer() const { 
    return player_;
}