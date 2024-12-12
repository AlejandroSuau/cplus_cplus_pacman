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
    const auto& cells = game_map_.GetCells();
    const auto half_cell_size = static_cast<float>(game_map_.GetCellSize()) / 2.f;
    for (const auto& cell : cells) {
        const auto spawn_type = kMapCollectables[cell.row][cell.col];
        auto x = cell.center.x;
        auto y = cell.center.y;
        if (spawn_type == 1) {
            collectables_.emplace_back(std::make_unique<Collectable>(
                ECollectableType::SMALL,
                kScoreSmall,
                SDL_FRect{x - kSizeSmallHalf, y - kSizeSmallHalf, kSizeSmall, kSizeSmall}));
        } else if (spawn_type == 2) {
            collectables_.emplace_back(std::make_unique<Collectable>(
                ECollectableType::BIG,
                kScoreBig,
                SDL_FRect{x - kSizeBigHalf, y - kSizeBigHalf, kSizeBig, kSizeBig}));
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