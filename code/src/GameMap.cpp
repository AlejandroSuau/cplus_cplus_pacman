#include "GameMap.hpp"

#include "Constants.hpp"

GameMap::GameMap(int width, int height, std::size_t cell_size) 
    : width_(width)
    , height_(height)
    , cells_size_(cell_size)
    , cell_size_int_(static_cast<int>(cells_size_)) {
    Init();
}
    
void GameMap::Init() {
    cols_count_ = static_cast<std::size_t>(width_ / cell_size_int_);
    rows_count_ = static_cast<std::size_t>(height_ / cell_size_int_);
    cells_count_ = cols_count_ * rows_count_;
    cells_.reserve(cells_count_);
    std::size_t index = 0;
    for (std::size_t row = 0; row < rows_count_; ++row) {
        for (std::size_t col = 0; col < cols_count_; ++col) {
            cells_.emplace_back(
                index,
                static_cast<int>(col * kCellSize),
                static_cast<int>(row * kCellSize));
        }
        ++index;
    }
}

void GameMap::Update() {
    
}

void GameMap::Render(SDL_Renderer& renderer) {
    SDL_SetRenderDrawColor(&renderer, 150, 150, 150, 255);
    for (auto& cell : cells_) {
        SDL_Rect cell_rect {
            cell.x_,
            cell.y_,
            cell_size_int_,
            cell_size_int_};
        
        SDL_SetRenderDrawColor(&renderer, 150, 150, 150, 255);
        SDL_RenderDrawRect(&renderer, &cell_rect);
        if (!cell.is_walkable_) {
            SDL_SetRenderDrawColor(&renderer, 220, 220, 220, 255);
            SDL_RenderFillRect(&renderer, &cell_rect);
        }
    }
}

void GameMap::Occupy(int row, int col) {
    // check if it is occupied?
    SetOccupy(row, col, true);
}

void GameMap::Unoccupy(int row, int col) {
    SetOccupy(row, col, false);
}

void GameMap::SetOccupy(int row, int col, bool is_occupied) {
    const auto index = FromRowColToIndex(row, col);
    cells_[index].is_occupied_ = is_occupied;
}

void GameMap::SetIsWalkable(int row, int col, bool is_walkable) {
    if (!AreRowColInsideBoundaries(row, col)) return;

    const auto index = FromRowColToIndex(row, col);
    cells_[index].is_walkable_ = is_walkable;
}

bool GameMap::AreCoordsWalkable(int x, int y) const {
    if (!AreCoordsInsideBoundaries(x, y)) return false;
    
    const auto [row, col] = FromCoordsToRowCol(x, y);
    const auto index = FromRowColToIndex(row, col);
    return cells_[index].is_walkable_;
}

bool GameMap::AreRowColWalkable(int row, int col) const {
    if (!AreRowColInsideBoundaries(row, col)) return false;

    const auto index = FromRowColToIndex(row, col);
    return cells_[index].is_walkable_;
}

bool GameMap::IsWalkable(std::size_t index) const {
    return (index < cells_count_ && cells_[index].is_walkable_);
}

bool GameMap::AreRowColInsideBoundaries(int row, int col) const {
    return (row >= 0 && row < rows_count_ && col >= 0 && col < cols_count_);
}

GameMap::CoordsPair GameMap::FromRowColToCoords(int row, int col) const {
    const auto index = FromRowColToIndex(row, col);
    return std::make_pair(cells_[index].x_, cells_[index].y_);
}

bool GameMap::AreCoordsInsideBoundaries(int x, int y) const {
    return (x >= 0 && x < width_ && y >= 0 && y < height_);
}

std::size_t GameMap::FromRowColToIndex(int row, int col) const {
    return (row * cols_count_ + col);
}

GameMap::RowColPair GameMap::FromIndexToRowCol(std::size_t index) const {
    return std::make_pair(index / cols_count_, index % cols_count_);
}

GameMap::RowColPair GameMap::FromCoordsToRowCol(int coord_x, int coord_y) const {
    return std::make_pair(coord_y / cell_size_int_, coord_x / cell_size_int_);
}

bool GameMap::IsInsideBoundaries(std::size_t index) const {
    return (index >= 0 && index < cells_count_);
}

std::size_t GameMap::GetRowsCount() const {
    return rows_count_;
}

std::size_t GameMap::GetColumnsCount() const {
    return cols_count_;
}

std::size_t GameMap::GetCellsCount() const {
    return cells_count_;
}
