#include "CollisionManager.hpp"

#include "scenes/GameScene.hpp"

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
    sounds_collect_[0] = sound_manager_.LoadSoundEffect(kAssetsFolderSounds + "eat_dot_0.wav");
    sounds_collect_[1] = sound_manager_.LoadSoundEffect(kAssetsFolderSounds + "eat_dot_1.wav");
}

void CollisionManager::CheckCollisions(GameScene& game_scene) {
    // Player - Collectable
    const auto& player_hitbox = player_.GetHitBox();
    auto& collectables = collectable_manager_.GetCollectableList();
    for (auto& collectable : collectables) {
        if (AreColliding(player_.GetHitBox(), collectable->hitbox)) {
            OnCollisionWithCollectable(*collectable, game_scene);
        }
    }

    // Player - Ghost
    for (auto& ghost : ghosts_) {
        if (AreColliding(player_hitbox, ghost->GetHitBox())) {
            OnCollisionWithGhost(*ghost, game_scene);
        }
    }
}

void CollisionManager::OnCollisionWithCollectable(Collectable& collectable, GameScene& game_scene) {
    Mix_PlayChannel(-1, sounds_collect_[sound_collect_index_], 0);
    sound_collect_index_ = !sound_collect_index_;

    player_.IncreaseScore(collectable.score);
    collectable.is_marked_for_destroy = true;
    if (collectable.type == ECollectableType::BIG) {
        game_scene.StartGhostFrightenedTimer();
    }
}

void CollisionManager::OnCollisionWithGhost(Ghost& ghost, GameScene& game_scene) {
    if (player_.IsDying() || ghost.IsInStateEyes()) return;

    if (ghost.IsInStateFrightened()) {
        game_scene.OnGhostDie(ghost);
    } else if (ghost.IsInStateChasing()) {
        game_scene.OnPlayerDie();
    }
}