#include "SoundPlayer.hpp"


SoundPlayer::SoundPlayer(SoundManager& sound_manager)
    : sound_manager_(sound_manager) {
    LoadSounds();
}

void SoundPlayer::PlayMusicIntro() {
    PlayMusic(0, false);
}

void SoundPlayer::PlayMusicPlaying() {
    PlayMusic(1, true); 
}

void SoundPlayer::PlayMusicEyes() {
    PlayMusic(2, true); 
}

void SoundPlayer::PlayMusicFrightened() {
    PlayMusic(3, true); 
}

void SoundPlayer::PlayMusicWin() {
    PlayMusic(4, false); 
}

void SoundPlayer::StopMusic() {
    Mix_HaltChannel(-1);
}

void SoundPlayer::PlaySoundDiePlayer() {
    Mix_PlayChannel(-1, sound_die_player_, 0);
}

void SoundPlayer::PlaySoundDieGhost() {
    Mix_PlayChannel(-1, sound_die_ghost_, 0);
};


void SoundPlayer::LoadSounds() {
    static const std::array<std::string, kAvailableSongs> song_names_ {
        "start.wav", "siren0_firstloop.wav", "eyes_firstloop.wav", "fright_firstloop.wav", "intermission.wav"};
    for (std::size_t i = 0; i < kAvailableSongs; ++i) {
        songs_[i] = sound_manager_.LoadSoundEffect(kAssetsFolderSounds + song_names_[i]);
    }
    
    sound_die_ghost_ = sound_manager_.LoadSoundEffect(kAssetsFolderSounds + "eat_ghost.wav");
    sound_die_player_ = sound_manager_.LoadSoundEffect(kAssetsFolderSounds + "death_0.wav");
}

void SoundPlayer::PlayMusic(std::size_t index, bool loop) {
    StopMusic();
    Mix_PlayChannel(index, songs_[index], loop ? -1 : 0);
}