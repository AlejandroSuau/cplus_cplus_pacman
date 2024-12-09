#include "CollectableManager.hpp"

#include "utils/Collisions.hpp"

#include "Constants.hpp"
#include "Player.hpp"

#include <array>
#include <algorithm>
#include <ranges>

namespace {
static const float kSizeSmall = 4.f;
static const float kSizeBig = 16.f;
static const unsigned int kScoreSmall = 10;
static const unsigned int kScoreBig = 100;
}

CollectableManager::CollectableManager(
    Renderer& renderer,
    TextureManager& texture_manager,
    const GameMap& game_map) 
    : renderer_(renderer)
    , texture_manager_(texture_manager)
    , game_map_(game_map)
    , texture_(nullptr) {
    Init();
}

void CollectableManager::Init() {
    texture_ = texture_manager_.LoadTexture(kAssetsFolderImages + "spritesheet.png");
    
    collectables_.clear();
    const auto& cells = game_map_.GetCells();
    const auto cell_size = static_cast<float>(game_map_.GetCellSize());
    for (const auto& cell : cells) {
        const auto spawn_type = kMapCollectables[cell.row][cell.col];
        if (spawn_type == 1) {
            collectables_.emplace_back(std::make_unique<Collectable>(
                ECollectableType::SMALL,
                kScoreSmall,
                cell.position.x + (cell_size / 2.f) - (kSizeSmall / 2.f),
                cell.position.y + (cell_size / 2.f) - (kSizeSmall / 2.f),
                kSizeSmall,
                kSizeSmall));
        } else if (spawn_type == 2) {
            collectables_.emplace_back(std::make_unique<Collectable>(
                ECollectableType::BIG,
                kScoreBig,
                cell.position.x + (cell_size / 2.f) - (kSizeBig / 2.f),
                cell.position.y + (cell_size / 2.f) - (kSizeBig / 2.f),
                kSizeBig,
                kSizeBig));
        }
    }
}

bool CollectableManager::DidCollectAll() const {
    return collectables_.empty();
}

void CollectableManager::MarkAllForDestroy() {
    for (auto& c : collectables_) {
        c->is_marked_for_destroy = true;
    }
}

void CollectableManager::RemoveCollectablesMarkedForDestroy() {
    auto should_remove_collectable = [](const auto& collectable) {
        return collectable->is_marked_for_destroy;
    };

    collectables_.erase(
        std::remove_if(collectables_.begin(), collectables_.end(), should_remove_collectable),
        collectables_.end()
    );
}

void CollectableManager::Render() {
    const SDL_Rect src_r {2, 182, 8, 8};
    for (const auto& c : collectables_) {
        renderer_.RenderTexture(texture_, src_r, c->hitbox);
    }
}

CollectableList& CollectableManager::GetCollectableList() {
    return collectables_;
}