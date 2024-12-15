#include "CollectableManager.hpp"

#include "utils/Collisions.hpp"

#include "Constants.hpp"
#include "Player.hpp"

#include <array>
#include <algorithm>
#include <ranges>

namespace {
static const float kSizeSmall = 4.f;
static const float kSizeSmallHalf = kSizeSmall / 2.f;

static const float kSizeBig = 16.f;
static const float kSizeBigHalf = kSizeBig / 2.f;

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
    
    texture_ = texture_manager_.LoadTexture(kAssetsFolderImages + "spritesheet.png");
    CreateCollectables();
}

void CollectableManager::CreateCollectables() {    
    collectables_.clear();
    const auto last_col = game_map_.GetColumnsCount() - 1;
    const auto last_row = game_map_.GetRowsCount() - 1;

    const auto& cells = game_map_.GetCells();
    const auto half_cell_size = static_cast<float>(game_map_.GetCellSize()) / 2.f;
    for (const auto& cell : cells) {
        const auto spawn_type = kMapCollectables[cell.row][cell.col];
        if (spawn_type == 0) continue;

        const auto x = cell.center.x;
        const auto y = cell.center.y;
        switch(spawn_type) {
            case 1: AddCollectable(ECollectableType::SMALL, kScoreSmall, kSizeSmall, x, y); break;
            case 2: AddCollectable(ECollectableType::BIG, kScoreBig, kSizeBig, x, y);       break;
        }

        // Adding more half right and half down if posible.
        bool add_coollectable_right = (
            cell.col != last_col && 
            (cell.col + 1) <= last_col && 
            kMapCollectables[cell.row][cell.col + 1]);
        if (add_coollectable_right) {
            AddCollectable(ECollectableType::SMALL, kScoreSmall, kSizeSmall, x + half_cell_size, y);
        }

        bool add_coollectable_down = (
            cell.row != last_row && 
            (cell.row + 1) <= last_row && 
            kMapCollectables[cell.row + 1][cell.col]);
        if (add_coollectable_down) {
            AddCollectable(ECollectableType::SMALL, kScoreSmall, kSizeSmall, x, y + half_cell_size);
        }
    }
}

void CollectableManager::AddCollectable(ECollectableType type, unsigned int score, float size, float x, float y) {
    collectables_.emplace_back(std::make_unique<Collectable>(
        type, score, SDL_FRect{x - size/2.f, y - size / 2.f, size, size}
    ));
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

unsigned int CollectableManager::GetAllCollectableScores() const {
    unsigned int total_score = 0;
    for (const auto& c : collectables_) {
        total_score += c->score;
    }
    return total_score;
}

CollectableList& CollectableManager::GetCollectableList() {
    return collectables_;
}