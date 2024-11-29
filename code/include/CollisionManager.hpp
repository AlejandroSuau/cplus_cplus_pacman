#pragma once

#include "Player.hpp"
#include "Ghost.hpp"
#include "CollectableManager.hpp"
#include "Types.hpp"

class CollisionManager {
public:
    CollisionManager(
        Player& player,
        GhostList& ghosts,
        CollectableManager& collectable_list);

    void CheckCollisions();

private:
    Player& player_;
    GhostList& ghosts_;
    CollectableManager& collectable_manager_;

    void OnCollisionWithCollectable(Collectable& collectable_type);
    void OnCollisionWithGhost(Ghost& ghost);    
};
