#include "Game.hpp"

#include "Constants.hpp"
#include "Player.hpp"
#include "Ghost.hpp"

#include "scenes/MainMenuScene.hpp"
#include "scenes/GameScene.hpp"

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
    , sdl_renderer_(
        SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED),
        SDL_DestroyRenderer)
    , is_running_(false)
    , renderer_(*sdl_renderer_)
    , texture_manager_(*sdl_renderer_.get())
    , scene_(nullptr)
    , swap_to_game_scene_(false) {
    if (!window_ || !sdl_renderer_) {
        throw std::runtime_error(
            std::string("Error creating the game") + SDL_GetError());
    }
    
    SDL_SetRenderDrawBlendMode(sdl_renderer_.get(), SDL_BLENDMODE_BLEND);
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
            Update(kFixedTimeStep / 1000.f); // Pass in seconds as a float
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
    // SetSceneGame();
    SetSceneMainMenu();
}

void Game::Update(float dt) {
    scene_->Update(dt);
    
    if (swap_to_game_scene_) {
        SetSceneGame();
    }
}

void Game::Render() {
    auto* renderer = sdl_renderer_.get();
    SDL_RenderClear(renderer);

    scene_->Render();

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);
}

void Game::HandleEvents() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            Shutdown();
            return;
        }

        const bool is_key_up = event.type == SDL_KEYDOWN;
        const bool is_key_down = event.type == SDL_KEYUP;
        if (is_key_down || is_key_up) {
            EEventKeyboard event_keyboard = is_key_down ? EEventKeyboard::DOWN : EEventKeyboard::UP;
            scene_->OnEventKeyboard(event_keyboard, event.key.keysym.scancode, *this);
        }
        
        const bool is_mouse_down = event.type == SDL_MOUSEBUTTONDOWN;
        const bool is_mouse_up = event.type == SDL_MOUSEBUTTONUP;
        const bool is_mouse_motion = event.type == SDL_MOUSEMOTION;
        if (is_mouse_down || is_mouse_up || is_mouse_motion) {
            EEventMouse event_mouse;
            if (is_mouse_down)    { event_mouse = EEventMouse::DOWN;   }
            else if (is_mouse_up) { event_mouse = EEventMouse::UP;     }
            else                  { event_mouse = EEventMouse::MOTION; }
            const Vec2<float> coords {
                static_cast<float>(event.button.x), static_cast<float>(event.button.y)};
            scene_->OnEventMouse(event_mouse, coords, *this);
        }
    }
}

void Game::SwapToGameScene() {
    swap_to_game_scene_ = true;
}

void Game::SetSceneGame() {
    scene_ = std::make_unique<GameScene>(
        renderer_, sound_manager_, texture_manager_, text_manager_);
    swap_to_game_scene_ = false;
}

void Game::SetSceneMainMenu() {
    scene_ = std::make_unique<MainMenuScene>(
        renderer_, sound_manager_, text_manager_, texture_manager_);
}

void Game::Shutdown() {
    is_running_ = false;
}
