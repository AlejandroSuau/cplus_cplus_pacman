#include "Ghost.hpp"
#include "Constants.hpp"

Ghost::Ghost(
    TextureManager& texture_manager,
    GameMap& game_map,
    Pathfinder& pathfinder,
    Vec2 position,
    SDL_Color color)
    : texture_manager_(texture_manager)
    , game_map_(game_map)
    , pathfinder_(pathfinder)
    , position_(position)
    , color_(color)
    , path_index_(0)
    , path_step_timer_(500) {}

void Ghost::FindPath(int target_row, int target_col) {
    path_index_ = 0;
    const auto [row, col] = game_map_.FromCoordsToRowCol(
        static_cast<int>(position_.x),
        static_cast<int>(position_.y));
    path_ = pathfinder_.FindPath(
        row,
        col,
        target_row,
        target_col);
}

void Ghost::Update(float dt) {
    if (!path_.empty() && path_step_timer_.HasElapsed()) {
        ++path_index_;
        if (path_index_ >= path_.size()) return;

        const auto row_col = path_[path_index_];
        const auto new_coords = game_map_.FromRowColToCoords(row_col.first, row_col.second);
        position_.x = new_coords.first;
        position_.y = new_coords.second;
    }
}

void Ghost::Render(SDL_Renderer& renderer) {
    for (std::size_t i = path_index_; i < path_.size(); ++i) {
        const auto& cell = path_[i];
        SDL_Rect cell_rect {
            cell.second * kCellSizeInt, //x
            cell.first * kCellSizeInt, //y
            kCellSizeInt,
            kCellSizeInt};
        
        SDL_SetRenderDrawColor(&renderer, color_.r, color_.g, color_.b, 50);
        SDL_RenderFillRect(&renderer, &cell_rect);
    }

    SDL_FRect rect {
        position_.x,
        position_.y,
        static_cast<int>(kCellSize),
        static_cast<int>(kCellSize)};
        
    SDL_SetRenderDrawColor(&renderer, color_.r, color_.g, color_.b, color_.a);
    SDL_RenderFillRectF(&renderer, &rect);
}

