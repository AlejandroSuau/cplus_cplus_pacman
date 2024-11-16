#include "GameMap.hpp"

#include "Constants.hpp"

GameMap::GameMap(int width, int height, int padding_x, int padding_y, std::size_t cell_size) 
    : width_(width)
    , height_(height)
    , padding_x_(padding_x)
    , padding_y_(padding_y)
    , cells_size_(cell_size)
    , cell_size_int_(static_cast<int>(cells_size_))
    , rows_count_(kRowsCount)
    , cols_count_(kColsCount)
    , cells_count_(rows_count_ * cols_count_) {
    Init();
}
    
void GameMap::Init() {
    std::size_t i = 0;
    std::size_t row_num = 0;
    int y = padding_y_;
    cells_.reserve(cells_count_);
    for (const auto& row : kMapTiles) {
        std::size_t col_num = 0;
        int x = padding_x_;
        for (const auto value : row) {
            const auto is_walkable = (value == 0);
            cells_.emplace_back(i, x, y, row_num, col_num, is_walkable);
            x += cell_size_int_;
            ++i;
            ++col_num;
        }
        ++row_num;
        y += cell_size_int_;
    }
}

void GameMap::Update() {
    
}

void GameMap::Render(SDL_Renderer& renderer) {
    SDL_SetRenderDrawColor(&renderer, 0, 100, 225, 100);
    for (auto& cell : cells_) {
        if (cell.is_walkable) continue;

        SDL_Rect cell_rect {
            cell.x,
            cell.y,
            cell_size_int_,
            cell_size_int_}; 
        SDL_RenderFillRect(&renderer, &cell_rect);
    }

    SDL_Rect limits_r {padding_x_, padding_y_, width_, height_};
    SDL_RenderDrawRect(&renderer, &limits_r);
}

void GameMap::SetIsWalkable(int row, int col, bool is_walkable) {
    if (!AreRowColInsideBoundaries(row, col)) return;

    const auto index = FromRowColToIndex(row, col);
    cells_[index].is_walkable = is_walkable;
}

bool GameMap::AreCoordsWalkable(int x, int y) const {
    if (!AreCoordsInsideBoundaries(x, y)) return false;
    
    const auto [row, col] = FromCoordsToRowCol(x, y);
    const auto index = FromRowColToIndex(row, col);
    return cells_[index].is_walkable;
}

bool GameMap::AreRowColWalkable(int row, int col) const {
    if (!AreRowColInsideBoundaries(row, col)) return false;

    const auto index = FromRowColToIndex(row, col);
    return cells_[index].is_walkable;
}

bool GameMap::IsWalkable(std::size_t index) const {
    return (index < cells_count_ && cells_[index].is_walkable);
}

bool GameMap::AreRowColInsideBoundaries(int row, int col) const {
    return (row >= 0 && row < rows_count_ && col >= 0 && col < cols_count_);
}

GameMap::CoordsPair GameMap::FromRowColToCoords(int row, int col) const {
    const auto index = FromRowColToIndex(row, col);
    return std::make_pair(cells_[index].x, cells_[index].y);
}

bool GameMap::AreCoordsInsideBoundaries(int x, int y) const {
    return (x >= padding_x_ && x <= (width_ + padding_x_) &&
            y >= padding_y_ && y <= (height_ + padding_y_));
}

std::size_t GameMap::FromRowColToIndex(int row, int col) const {
    return (row * cols_count_ + col);
}

GameMap::RowColPair GameMap::FromIndexToRowCol(std::size_t index) const {
    return std::make_pair(index / cols_count_, index % cols_count_);
}

GameMap::RowColPair GameMap::FromCoordsToRowCol(int coord_x, int coord_y) const {
    return std::make_pair((coord_y - padding_x_) / cell_size_int_, 
                          (coord_x  - padding_y_) / cell_size_int_) ;
}

bool GameMap::IsInsideBoundaries(std::size_t index) const {
    return (index < cells_count_);
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

const std::vector<GameMap::Cell>& GameMap::GetCells() const {
    return cells_;
}

std::size_t GameMap::GetCellSize() const {
    return cells_size_;
}

int GameMap::GetCellSizeInt() const {
    return cell_size_int_;
}
