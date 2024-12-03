#include "GameMap.hpp"

#include "Constants.hpp"

#include <algorithm>

GameMap::GameMap(float width, float height, Vec2<float> padding, std::size_t cell_size) 
    : width_(width)
    , height_(height)
    , padding_(padding)
    , cell_size_(cell_size)
    , cell_size_int_(static_cast<int>(cell_size_))
    , cell_size_float_(static_cast<float>(cell_size_))
    , rows_count_(kRowsCount)
    , cols_count_(kColsCount)
    , rows_count_int_(static_cast<int>(rows_count_))
    , cols_count_int_(static_cast<int>(cols_count_))
    , cells_count_(rows_count_ * cols_count_) {
    Init();
}
    
void GameMap::Init() {
    std::size_t i = 0;
    std::size_t row_num = 0;
    Vec2<float> pos {0.f, padding_.y};
    cells_.reserve(cells_count_);
    for (const auto& row : kMapTiles) {
        std::size_t col_num = 0;
        pos.x = padding_.x;
        for (const auto value : row) {
            const auto is_walkable = (value == 0);
            const auto center = pos + Vec2{cell_size_float_ / 2.f, cell_size_float_ / 2.f};
            cells_.emplace_back(i, pos, center, row_num, col_num, is_walkable);
            pos.x += cell_size_float_;
            ++i;
            ++col_num;
        }
        ++row_num;
        pos.y += cell_size_float_;
    }
}

void GameMap::Update() {
    
}

void GameMap::Render(SDL_Renderer& renderer) {
    SDL_SetRenderDrawColor(&renderer, 0, 100, 225, 100);
    for (auto& cell : cells_) {
        //if (cell.is_walkable) continue;

        SDL_FRect cell_rect {
            cell.position.x,
            cell.position.y,
            cell_size_float_,
            cell_size_float_}; 
        //SDL_RenderDrawRectF(&renderer, &cell_rect);            
        if (!cell.is_walkable) SDL_RenderFillRectF(&renderer, &cell_rect);
    }

    SDL_FRect limits_r {padding_.x, padding_.y, width_, height_};
    SDL_RenderDrawRectF(&renderer, &limits_r);
}

void GameMap::SetIsWalkable(Vec2<int> col_row, bool is_walkable) {
    if (!AreColRowInsideBoundaries(col_row)) return;

    const auto index = FromColRowToIndex(col_row);
    cells_[index].is_walkable = is_walkable;
}

bool GameMap::AreCoordsWalkable(Vec2<float> coords) const {
    if (!AreCoordsInsideBoundaries(coords)) return false;
    
    const auto col_row = FromCoordsToColRow(coords);
    const auto index = FromColRowToIndex(col_row);
    return cells_[index].is_walkable;
}

bool GameMap::AreColRowWalkable(Vec2<int> col_row) const {
    if (!AreColRowInsideBoundaries(col_row)) return false;

    const auto index = FromColRowToIndex(col_row);
    return cells_[index].is_walkable;
}

bool GameMap::IsWalkable(std::size_t index) const {
    return (index < cells_count_ && cells_[index].is_walkable);
}

bool GameMap::AreColRowInsideBoundaries(Vec2<int> col_row) const {
    return (col_row.y >= 0 && col_row.y < rows_count_int_ &&
            col_row.x >= 0 && col_row.x < cols_count_int_);
}

void GameMap::ClampColRowIntoMapDimensions(Vec2<int>& col_row) const {
    col_row.x = std::clamp(col_row.x, 0, static_cast<int>(GetColumnsCount()) - 1);
    col_row.y = std::clamp(col_row.y, 0, static_cast<int>(GetRowsCount()) - 1);
}

Vec2<float> GameMap::FromColRowToCoords(Vec2<int> col_row) const {
    const auto index = FromColRowToIndex(col_row);
    return cells_[index].position;
}

bool GameMap::AreCoordsInsideBoundaries(Vec2<float> coords) const {
    return (coords.x >= padding_.x && coords.x < (width_ + padding_.x) &&
            coords.y >= padding_.y && coords.y < (height_ + padding_.y));
}

std::size_t GameMap::FromColRowToIndex(Vec2<int> col_row) const {
    return (col_row.y * cols_count_ + col_row.x);
}

Vec2<int> GameMap::FromIndexToColRow(std::size_t index) const {
    return {static_cast<int>(index / cols_count_),
            static_cast<int>(index % cols_count_)};
}

Vec2<int> GameMap::FromCoordsToColRow(Vec2<float> coords) const {
    return {static_cast<int>(coords.x - padding_.x) / cell_size_int_,
            static_cast<int>(coords.y - padding_.y) / cell_size_int_};
}

Vec2<float> GameMap::FromCoordsToCenterCellCoords(Vec2<float> coords) const {
    const auto& cell = GetCell(coords);
    return cell.center;
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

const GameMap::Cell& GameMap::GetCell(Vec2<int> col_row) const {
    const auto index = FromColRowToIndex(col_row);
    return cells_[index];
}

const GameMap::Cell& GameMap::GetCell(Vec2<float> coords) const {
    const auto col_row = FromCoordsToColRow(coords);
    const auto index = FromColRowToIndex(col_row);
    return cells_[index];
}

const std::vector<GameMap::Cell>& GameMap::GetCells() const {
    return cells_;
}

std::size_t GameMap::GetCellSize() const {
    return cell_size_;
}

int GameMap::GetCellSizeInt() const {
    return cell_size_int_;
}

float GameMap::GetCellSizeFloat() const {
    return cell_size_float_;
}
