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
    auto is_big_spawn = [](std::size_t row, std::size_t col) {
        static const std::array<std::pair<std::size_t, std::size_t>, 4> big_spawns {{
            {2, 1}, {2, 15}, {15, 1}, {15, 15}
        }};
        for (const auto [r, c] : big_spawns) {
            if (r == row && c == col) return true;
        }
        return false;
    };

    // TODO: CREATE FACTORY
    auto is_walkable_cell = [&](const GameMap::Cell& cell) { return (cell.is_walkable /* TODO: || players.position() */); };
    const auto& cells = game_map_.GetCells();
    const auto cell_size = game_map_.GetCellSizeInt();
    for (const auto& cell : cells | std::views::filter(is_walkable_cell)) {
        if (is_big_spawn(cell.row, cell.col)) {
            collectables_.emplace_back(std::make_unique<Collectable>(
                ECollectableType::BIG,
                kScoreBig,
                cell.x + (cell_size / 2) - (kSizeBig / 2),
                cell.y + (cell_size / 2) - (kSizeBig / 2),
                kSizeBig,
                kSizeBig));
        } else {
            collectables_.emplace_back(std::make_unique<Collectable>(
                ECollectableType::SMALL,
                kScoreSmall,
                cell.x + (cell_size / 2) - (kSizeSmall / 2),
                cell.y + (cell_size / 2) - (kSizeSmall / 2),
                kSizeSmall,
                kSizeSmall));
        }
    }
}

/*std::optional<CollectableManager::ECollectableType> CollectableManager::ProcessCollisions(Game& game) {
    for (auto& c : collectables_) {
        const auto& p_rect = game.GetPlayer().GetHitbox();
        if (AreColliding(c->rect, p_rect)) {
            OnCollision(*c);
            return {c->type};
        }
    }

    return std::nullopt;
}*/

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