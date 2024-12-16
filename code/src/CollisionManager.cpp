#include "CollisionManager.hpp"

#include "utils/Collisions.hpp"

#include "Constants.hpp"

CollisionManager::CollisionManager(
    SoundManager& sound_manager,
    Player& player,
    GhostList& ghosts,
    CollectableManager& collectable_manager)
    : sound_manager_(sound_manager)
    , player_(player)
    , ghosts_(ghosts)
    , collectable_manager_(collectable_manager)
    , sound_collect_index_(0) {
    LoadSounds();
}

void CollisionManager::LoadSounds() {
    sound_die_player_ = sound_manager_.LoadSoundEffect(kAssetsFolderSounds + "death_0.wav");
    sound_die_ghost_ = sound_manager_.LoadSoundEffect(kAssetsFolderSounds + "eat_ghost.wav");

    sounds_collect_[0] = sound_manager_.LoadSoundEffect(kAssetsFolderSounds + "eat_dot_0.wav");
    sounds_collect_[1] = sound_manager_.LoadSoundEffect(kAssetsFolderSounds + "eat_dot_1.wav");
}

void CollisionManager::CheckCollisions() {
    // Player - Collectable
    const auto& player_hitbox = player_.GetHitBox();
    auto& collectables = collectable_manager_.GetCollectableList();
    for (auto& collectable : collectables) {
        if (AreColliding(player_.GetHitBox(), collectable->hitbox)) {
            OnCollisionWithCollectable(*collectable);
        }
    }

    // Player - Ghost
    for (auto& ghost : ghosts_) {
        if (AreColliding(player_hitbox, ghost->GetHitBox())) {
            OnCollisionWithGhost(*ghost);
        }
    }
}

void CollisionManager::OnCollisionWithCollectable(Collectable& collectable) {
    Mix_PlayChannel(-1, sounds_collect_[sound_collect_index_], 0);
    sound_collect_index_ = !sound_collect_index_;

    player_.IncreaseScore(collectable.score);
    collectable.is_marked_for_destroy = true;
    if (collectable.type == ECollectableType::BIG) {
        for (auto& ghost : ghosts_) {
            ghost->SetStateFrightened();
        }
    }
}

void CollisionManager::OnCollisionWithGhost(Ghost& ghost) {
    if (player_.IsDying() || ghost.IsInStateEyes()) return;

    if (ghost.IsInStateFrightened()) {
        Mix_PlayChannel(-1, sound_die_ghost_, 0);
        ghost.Die();
    } else if (ghost.IsInStateChasing()) {
        Mix_PlayChannel(-1, sound_die_player_, 0);
        player_.Die();
        for (auto& ghost : ghosts_) {
            ghost->SetStateStop();
        }
    }
}