#include "pathfinder/Pathfinder.hpp"

#include <functional>
#include <algorithm>

#include "GameMap.hpp"

Pathfinder::Pathfinder(GameMap& map)
    : map_(map)
    , target_index_(0)
    , start_row_(0)
    , start_col_(0)
    , target_col_(0)
    , target_row_(0)
    , target_node_(nullptr)
    , did_finish_(false)
    , target_nodes_count_(0) {

    Reset(0, 0, 0, 0);
}


Pathfinder::Path Pathfinder::FindPath(
    int start_row,
    int start_col,
    int target_row,
    int target_col) {

    Reset(start_row, start_col, target_row, target_col);
    while (!did_finish_) {
        Step();
    }

    return ReconstructPath();
}

void Pathfinder::Reset(int start_row, int start_col, int target_row, int target_col) {
    start_row_ = start_row;
    start_col_ = start_col;
    target_row_ = target_row;
    target_col_ = target_col;
    
    did_finish_ = false;
    target_node_ = nullptr;
    
    target_index_ = map_.FromRowColToIndex(target_row_, target_col_);

    // Map Nodes
    open_nodes_.clear();
    map_nodes_.clear();
    target_nodes_count_ = 0;
    const auto map_cells_count = map_.GetCellsCount();
    map_nodes_.reserve(map_cells_count);
    for (std::size_t i = 0; i < map_cells_count; ++i) {
        map_nodes_.emplace_back(i);
    }

    // Nodes Open
    // TODO: Crear método para reiniciarla
    // nodes_open_ = std::priority_queue<Pathfinder::MapNode*>();
    const auto node_index = map_.FromRowColToIndex(start_row_, start_col_);
    target_index_ = map_.FromRowColToIndex(target_row_, target_col_);
    auto& starting_node = map_nodes_[node_index];
    starting_node.h = Heuristic(start_row, start_col, target_row_, target_col_);
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
    const auto [cr, cc] = map_.FromIndexToRowCol(node->map_index);
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
    auto [row, col] = map_.FromIndexToRowCol(node->map_index);
    for (auto* neighbour : neighbours) {
        const bool skip_neighbour = (!neighbour || neighbour->is_closed);
        if (skip_neighbour) continue;

        const int g_cost = node->g + 1; // (weight * heuristic) 1 in our case
        if (!neighbour->is_open || g_cost < neighbour->g) {
            if (neighbour->is_open) {
                open_nodes_.erase(neighbour);
            }
            
            neighbour->g = g_cost;
            const auto [nrow, ncol] = map_.FromIndexToRowCol(neighbour->map_index);
            neighbour->h = Heuristic(nrow, ncol, target_row_, target_col_);
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

int Pathfinder::Heuristic(int row1, int col1, int row2, int col2) const {
    return std::abs(row1 - row2) + std::abs(col1 - col2);
}

Pathfinder::Path Pathfinder::ReconstructPath() const {
    Pathfinder::Path path;

    MapNode* current_node = target_node_;
    while (current_node) {
        const auto [row, col] = map_.FromIndexToRowCol(current_node->map_index);
        path.push_back(std::make_pair(row, col));
        current_node = current_node->parent;
    }

    std::reverse(path.begin(), path.end());
    return path;
}

void Pathfinder::Render(SDL_Renderer& renderer) {
    SDL_Rect starter_rect {start_col_ * 16, start_row_ * 16, 16, 16};
    SDL_SetRenderDrawColor(&renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(&renderer, &starter_rect);

    SDL_Rect target_rect {target_col_ * 16, target_row_ * 16, 16, 16};
    SDL_SetRenderDrawColor(&renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(&renderer, &target_rect);
}

bool Pathfinder::DidFinish() const {
    return did_finish_;
}