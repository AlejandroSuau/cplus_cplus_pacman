#include "CollectableManager.hpp"

#include "utils/Collisions.hpp"

#include "Constants.hpp"
#include "Player.hpp"

#include <array>
#include <algorithm>
#include <ranges>

namespace {
static const int kSizeSmall = 2;
static const int kSizeBig = 16;
static const unsigned int kScoreSmall = 5;
static const unsigned int kScoreBig = 100;
}

CollectableManager::CollectableManager(
    TextureManager& texture_manager,
    const GameMap& game_map) 
    : texture_manager_(texture_manager)
    , game_map_(game_map)
    , texture_(nullptr) {
    Init();
}

void CollectableManager::Init() {
    texture_ = texture_manager_.LoadTexture(kAssetsFolderImages + "spritesheet.png");

    const auto& cells = game_map_.GetCells();
    const auto cell_size = game_map_.GetCellSizeInt();
    for (const auto& cell : cells) {
        const auto spawn_type = kMapCollectables[cell.row][cell.col];
        if (spawn_type == 1) {
            collectables_.emplace_back(std::make_unique<Collectable>(
                ECollectableType::SMALL,
                kScoreSmall,
                cell.x + (cell_size / 2) - (kSizeSmall / 2),
                cell.y + (cell_size / 2) - (kSizeSmall / 2),
                kSizeSmall,
                kSizeSmall));
        } else if (spawn_type == 2) {
            collectables_.emplace_back(std::make_unique<Collectable>(
                ECollectableType::BIG,
                kScoreBig,
                cell.x + (cell_size / 2) - (kSizeBig / 2),
                cell.y + (cell_size / 2) - (kSizeBig / 2),
                kSizeBig,
                kSizeBig));
        }
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

void CollectableManager::Render(SDL_Renderer& renderer) {
    const SDL_Rect src_r {2, 182, 8, 8};
    for (const auto& c : collectables_) {
        SDL_RenderCopyEx(&renderer, texture_, &src_r, &c->hitbox, 0, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
    }
}

CollectableList& CollectableManager::GetCollectableList() {
    return collectables_;
}