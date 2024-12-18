#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "utils/SoundManager.hpp"

#include "Constants.hpp"

#include <array>

class SoundPlayer {
public:
    SoundPlayer(SoundManager& sound_manager);
    
    void PlayMusicIntro();     
    void PlayMusicPlaying();
    void PlayMusicEyes();  
    void PlayMusicFrightened();
    void PlayMusicWin();       

    void StopMusic();
    void PlaySoundDiePlayer();
    void PlaySoundDieGhost();

private:
    SoundManager& sound_manager_;

    static const std::size_t kAvailableSongs = 5;
    std::array<Mix_Chunk*, kAvailableSongs> songs_{};

    Mix_Chunk* sound_die_ghost_ {nullptr};
    Mix_Chunk* sound_die_player_ {nullptr};

    void LoadSounds();
    void PlayMusic(std::size_t index, bool loop = true);
};