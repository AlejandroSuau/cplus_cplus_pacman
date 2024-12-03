#include "utils/Entity.hpp"


Entity::Entity(Renderer& renderer, SDL_FRect renderer_rect, float hitbox_scale)
    : renderer_(renderer), renderer_rect_(renderer_rect), starting_hitbox_(renderer_rect), hitbox_scale_(hitbox_scale) {
    UpdateHitBox();
}

void Entity::ResetHitBox() {
    renderer_rect_ = starting_hitbox_;
    UpdateHitBox();
}

const SDL_FRect& Entity::GetRendererRect() const {
    return renderer_rect_;
}

const SDL_FRect& Entity::GetHitBox() const {
    return hitbox_;
}

Vec2<float> Entity::GetPosition() const {
    return {hitbox_.x, hitbox_.y};
}

Vec2<float> Entity::GetCenterPosition() const {
    return {renderer_rect_.w / 2.f + renderer_rect_.x,
            renderer_rect_.h / 2.f + renderer_rect_.y};
}

void Entity::UpdatePosition(Vec2<float> new_coords) {
    renderer_rect_.x = new_coords.x;
    renderer_rect_.y = new_coords.y;

    UpdateHitBox();
}

void Entity::UpdateHitBox() {
    const float diff_width = renderer_rect_.w * (1.0f - hitbox_scale_) / 2.0f;
    const float diff_height = renderer_rect_.h * (1.0f - hitbox_scale_) / 2.0f;

    hitbox_.x = renderer_rect_.x + diff_width;
    hitbox_.y = renderer_rect_.y + diff_height;
    hitbox_.w = renderer_rect_.w * hitbox_scale_;
    hitbox_.h = renderer_rect_.h * hitbox_scale_;
}