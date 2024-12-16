#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "Player.hpp"
#include "Ghost.hpp"
#include "CollectableManager.hpp"
#include "Types.hpp"

#include "utils/SoundManager.hpp"

#include <array>

class CollisionManager {
public:
    CollisionManager(
        SoundManager& sound_manager,
        Player& player,
        GhostList& ghosts,
        CollectableManager& collectable_list);

    void CheckCollisions();

private:
    SoundManager& sound_manager_;
    Player& player_;
    GhostList& ghosts_;
    CollectableManager& collectable_manager_;

    Mix_Chunk* sound_die_player_ {nullptr};
    Mix_Chunk* sound_die_ghost_ {nullptr};

    std::size_t sound_collect_index_;
    std::array<Mix_Chunk*, 2> sounds_collect_ {nullptr, nullptr};

    void OnCollisionWithCollectable(Collectable& collectable_type);
    void OnCollisionWithGhost(Ghost& ghost);

    void LoadSounds();
};
