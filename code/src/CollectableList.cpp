#include "CollectableList.hpp"

#include "utils/Collisions.hpp"

#include "Constants.hpp"
#include "Game.hpp"
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

CollectableList::CollectableList(
    TextureManager& texture_manager,
    ScoreManager& score_manager,
    const GameMap& game_map) 
    : texture_manager_(texture_manager)
    , score_manager_(score_manager)
    , game_map_(game_map) {
    Init();
}

void CollectableList::Init() {
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

void CollectableList::ProcessCollisions(Game& game) {
    for (auto& c : collectables_) {
        const SDL_Rect c_rect {c->x, c->y, c->w, c->h};
        const auto& p_rect = game.GetPlayer().GetHitbox();
        if (AreColliding(c_rect, p_rect)) {
           OnCollision(*c);
        }
    }
}

void CollectableList::OnCollision(Collectable& collectable) {
    collectable.is_marked_for_destroy = true;
    score_manager_.IncreaseScore(static_cast<int>(collectable.score));
    if (collectable.type == ECollectableType::BIG) {
        // make vulnerables ghosts
    }
}

void CollectableList::RemoveCollectablesMarkedForDestroy() {
    auto should_remove_collectable = [](const auto& collectable) { return collectable->is_marked_for_destroy; };
    collectables_.erase(
        std::remove_if(collectables_.begin(), collectables_.end(), should_remove_collectable),
        collectables_.end()
    );
}

void CollectableList::Render(SDL_Renderer& renderer) {
    const SDL_Rect src_r {2, 182, 8, 8};
    for (const auto& c : collectables_) {
        SDL_Rect dst_r{c->x, c->y, c->w, c->h};
        SDL_RenderCopyEx(&renderer, texture_, &src_r, &dst_r, 0, nullptr, SDL_RendererFlip::SDL_FLIP_NONE);
    }
}