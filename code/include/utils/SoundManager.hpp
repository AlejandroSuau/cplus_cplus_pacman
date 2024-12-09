#pragma once

#include <SDL2/SDL_mixer.h>

#include <map>
#include <string>

class SoundManager {
public:
    ~SoundManager();

    Mix_Chunk* LoadSoundEffect(const std::string& file_path);
    Mix_Music* LoadMusic(const std::string& file_path);
    void RemoveSoundEffect(const std::string& file_path);
    void RemoveMusic(const std::string& file_path);

private:
    std::map<std::string, Mix_Chunk*> sound_effects_;
    std::map<std::string, Mix_Music*> music_;

    void ClearAllSounds();
};