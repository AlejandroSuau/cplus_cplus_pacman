#include "utils/Entity.hpp"


Entity::Entity(Renderer& renderer, SDL_Rect hitbox)
    : renderer_(renderer), hitbox_(hitbox), starting_hitbox_(hitbox_) {}

void Entity::ResetHitBox() {
    hitbox_ = starting_hitbox_;
}

const SDL_Rect& Entity::GetHitBox() const {
    return hitbox_;
}

Vec2 Entity::GetPosition() const {
    return {hitbox_.x, hitbox_.y};
}
