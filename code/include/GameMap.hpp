#pragma once

#include <SDL2/SDL.h>

#include "utils/Vec2.hpp"

#include <vector>
#include <utility>

class GameMap {
public:
    struct Cell {
        std::size_t cell_index;
        Vec2<float> position;
        Vec2<float> center;
        std::size_t row;
        std::size_t col;
        bool is_walkable;
        Cell(std::size_t cell_index_, Vec2<float> position_, Vec2<float> center_, std::size_t row_, std::size_t col_, bool is_walkable_)
            : cell_index(cell_index_), position(position_), center(center_), row(row_), col(col_), is_walkable(is_walkable_) {}
    };

    GameMap(
        float width,
        float height,
        Vec2<float> padding,
        std::size_t cell_size);

    void Init();
    void Update();
    void Render(SDL_Renderer& renderer);

    void SetIsWalkable(Vec2<int> col_row, bool is_walkable);
    bool AreColRowWalkable(Vec2<int> col_row) const;
    bool IsWalkable(std::size_t index) const;
    bool AreCoordsWalkable(Vec2<float> coords) const;
    void ClampColRowIntoMapDimensions(Vec2<int>& col_row) const;

    std::size_t FromColRowToIndex(Vec2<int> col_row) const;
    Vec2<float> FromColRowToCoords(Vec2<int> col_row) const;

    Vec2<int> FromIndexToColRow(std::size_t index) const;
    Vec2<int> FromCoordsToColRow(Vec2<float> coords) const;
    Vec2<float> FromCoordsToCenterCellCoords(Vec2<float> coords) const;

    std::size_t GetRowsCount() const;
    std::size_t GetColumnsCount() const;
    std::size_t GetCellsCount() const;
    std::size_t GetCellSize() const;
    int GetCellSizeInt() const;
    float GetCellSizeFloat() const;

    bool AreColRowInsideBoundaries(Vec2<int> col_row) const;
    bool AreCoordsInsideBoundaries(Vec2<float> coords) const;

    const Cell& GetCell(Vec2<int> col_row) const;
    const Cell& GetCell(Vec2<float> coords) const;
    const std::vector<Cell>& GetCells() const;

private:
    float width_;
    float height_;
    Vec2<float> padding_;
    std::size_t cell_size_;
    int cell_size_int_;
    float cell_size_float_;
    std::size_t rows_count_;
    std::size_t cols_count_;
    int rows_count_int_;
    int cols_count_int_;
    std::size_t cells_count_;

    std::vector<Cell> cells_;

    bool IsInsideBoundaries(std::size_t index) const;
};
