#include "pathfinder/Pathfinder.hpp"

#include <functional>
#include <algorithm>

#include "GameMap.hpp"

Pathfinder::Pathfinder(GameMap& map)
    : map_(map)
    , target_index_(0)
    , target_node_(nullptr)
    , did_finish_(false)
    , target_nodes_count_(0) {

    Reset();
}


Pathfinder::Path Pathfinder::FindPath(Vec2<int> col_row_from, Vec2<int> col_row_to) {
    Reset(col_row_from, col_row_to);
    while (!did_finish_) {
        Step();
    }

    return ReconstructPath();
}

void Pathfinder::Reset(Vec2<int> col_row_from, Vec2<int> col_row_to) {
    col_row_from_ = col_row_from;
    col_row_to_ = col_row_to;
    
    did_finish_ = false;
    target_node_ = nullptr;
    
    target_index_ = map_.FromColRowToIndex(col_row_to_);

    // Map Nodes
    open_nodes_.clear();
    map_nodes_.clear();
    target_nodes_count_ = 0;
    const auto map_cells_count = map_.GetCellsCount();
    map_nodes_.reserve(map_cells_count);
    for (std::size_t i = 0; i < map_cells_count; ++i) {
        map_nodes_.emplace_back(i);
    }

    const auto node_index = map_.FromColRowToIndex(col_row_from_);
    target_index_ = map_.FromColRowToIndex(col_row_to_);
    auto& starting_node = map_nodes_[node_index];
    starting_node.h = Heuristic(col_row_from_, col_row_to_);
    starting_node.is_closed = true;

    open_nodes_.emplace(&starting_node);
}

void Pathfinder::Step() {
    if (did_finish_ || open_nodes_.empty()) {
        did_finish_ = true;
        return;
    }

    auto* node = *open_nodes_.begin();
    open_nodes_.erase(open_nodes_.begin());
    const auto [cr, cc] = map_.FromIndexToColRow(node->map_index);
    if (node->map_index == target_index_) {
        target_node_ = node;
        did_finish_ = true;
        return;
    }

    if (!target_node_ || node->h < target_node_->h) {
        target_node_ = node;
    }

    node->is_closed = true;
    auto neighbours = GetNeighbours(node->map_index);
    auto [row, col] = map_.FromIndexToColRow(node->map_index);
    for (auto* neighbour : neighbours) {
        const bool skip_neighbour = (!neighbour || neighbour->is_closed);
        if (skip_neighbour) continue;

        const int g_cost = node->g + 1; // (weight * heuristic) 1 in our case
        if (!neighbour->is_open || g_cost < neighbour->g) {
            if (neighbour->is_open) {
                open_nodes_.erase(neighbour);
            }
            
            neighbour->g = g_cost;
            const auto n_col_row = map_.FromIndexToColRow(neighbour->map_index);
            neighbour->h = Heuristic(n_col_row, col_row_to_);
            neighbour->parent = node;

            neighbour->is_open = true;
            open_nodes_.emplace(neighbour);
        }
    }
}

Pathfinder::Neighbours Pathfinder::GetNeighbours(std::size_t node_index) {
    Neighbours neighbours {nullptr, nullptr, nullptr, nullptr};

    const int columns_count = static_cast<int>(map_.GetColumnsCount());
    const int index = static_cast<int>(node_index);

    const int e_index = index + 1;
    if (map_.IsWalkable(e_index) && e_index % columns_count != 0) neighbours[0] = &map_nodes_[e_index];
    
    const int w_index = index + -1;
    if (map_.IsWalkable(w_index) && w_index % columns_count != columns_count - 1) neighbours[1] = &map_nodes_[w_index];

    const int n_index = index - columns_count;
    if (map_.IsWalkable(n_index) && n_index >= 0) neighbours[2] = &map_nodes_[n_index];

    const int nodes_count = static_cast<int>(map_.GetCellsCount());
    const int s_index = index + columns_count;
    if (map_.IsWalkable(s_index) && s_index < nodes_count) neighbours[3] = &map_nodes_[s_index];

    return neighbours;
}

int Pathfinder::Heuristic(Vec2<int> col_row_left, Vec2<int> col_row_right) const {
    return std::abs(col_row_left.y - col_row_right.y) + std::abs(col_row_left.x - col_row_right.x);
}

Pathfinder::Path Pathfinder::ReconstructPath() const {
    Pathfinder::Path path;

    MapNode* current_node = target_node_;
    while (current_node) {
        const auto [row, col] = map_.FromIndexToColRow(current_node->map_index);
        path.emplace_back(col, row);
        current_node = current_node->parent;
    }

    std::reverse(path.begin(), path.end());
    return path;
}

void Pathfinder::Render(SDL_Renderer& renderer) {
    SDL_Rect starter_rect {col_row_from_.x * 16, col_row_from_.y * 16, 16, 16};
    SDL_SetRenderDrawColor(&renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(&renderer, &starter_rect);

    SDL_Rect target_rect {col_row_to_.x * 16, col_row_to_.y * 16, 16, 16};
    SDL_SetRenderDrawColor(&renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(&renderer, &target_rect);
}

bool Pathfinder::DidFinish() const {
    return did_finish_;
}