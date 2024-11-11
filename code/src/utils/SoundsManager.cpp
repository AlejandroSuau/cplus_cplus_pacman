#include "utils/SoundsManager.hpp"

#include <SDL2/SDL.h>

SoundsManager::~SoundsManager() {
    ClearAllSounds();
}

Mix_Chunk* SoundsManager::LoadSoundEffect(const std::string& file_path) {
    if (sound_effects_.count(file_path) == 0) {
        Mix_Chunk* sound = Mix_LoadWAV(file_path.c_str());
        if (!sound) {
            SDL_Log("Failed to load sound effect: %s. SDL_mixer Error: %s", file_path.c_str(), Mix_GetError());
            return nullptr;
        }
        sound_effects_[file_path] = sound;
    }
    return sound_effects_[file_path];
}

Mix_Music* SoundsManager::LoadMusic(const std::string& file_path) {
    if (music_.count(file_path) == 0) {
        Mix_Music* music = Mix_LoadMUS(file_path.c_str());
        if (!music) {
            SDL_Log("Failed to load music: %s. SDL_mixer Error: %s", file_path.c_str(), Mix_GetError());
            return nullptr;
        }
        music_[file_path] = music;
    }
    return music_[file_path];
}

void SoundsManager::RemoveSoundEffect(const std::string& file_path) {
    auto it = sound_effects_.find(file_path);
    if (it != sound_effects_.end()) {
        Mix_FreeChunk(it->second);
        sound_effects_.erase(it);
    }
}

void SoundsManager::RemoveMusic(const std::string& file_path) {
    auto it = music_.find(file_path);
    if (it != music_.end()) {
        Mix_FreeMusic(it->second);
        music_.erase(it);
    }
}

void SoundsManager::ClearAllSounds() {
    for (auto& sound_pair : sound_effects_) {
        Mix_FreeChunk(sound_pair.second);
    }
    sound_effects_.clear();

    for (auto& music_pair : music_) {
        Mix_FreeMusic(music_pair.second);
    }
    music_.clear();
}