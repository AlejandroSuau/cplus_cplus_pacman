#include "CollisionManager.hpp"

#include "utils/Collisions.hpp"

CollisionManager::CollisionManager(
    Player& player,
    GhostList& ghosts,
    CollectableManager& collectable_manager)
    : player_(player)
    , ghosts_(ghosts)
    , collectable_manager_(collectable_manager) {}

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
        ghost.Die();
    } else if (ghost.IsInStateChasing()) {
        player_.Die();
        for (auto& ghost : ghosts_) {
            ghost->SetStateStop();
        }
    }
}