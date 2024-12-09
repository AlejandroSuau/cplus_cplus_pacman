#pragma once

#include <SDL2/SDL.h>

#include "utils/SDLInitializer.hpp"
#include "utils/SDLImageInitializer.hpp"
#include "utils/SDLTTFInitializer.hpp"
#include "utils/SDLMixerInitializer.hpp"
#include "utils/CountdownTimer.hpp"
#include "utils/Renderer.hpp"
#include "utils/TextureManager.hpp"
#include "utils/TextManager.hpp"
#include "utils/SoundManager.hpp"

#include "Types.hpp"
#include "scenes/IScene.hpp"

#include <memory>

class Game {
public:
    Game();
    
    void Run();
    void Shutdown();

    void SwapToGameScene();

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
    SoundManager sound_manager_;
    TextureManager texture_manager_;
    TextManager text_manager_;
    std::unique_ptr<IScene> scene_;
    bool swap_to_game_scene_;
    
    void Init();

    void Update(float dt);
    void Render();
    void HandleEvents();

    void SetSceneGame();
    void SetSceneMainMenu();
};
