#pragma once

#include <SDL2/SDL.h>

#include "utils/Vec2.hpp"

#include <vector>
#include <utility>

class GameMap {
public:
    struct Cell {
        std::size_t cell_index;
        int x;
        int y;
        std::size_t row;
        std::size_t col;
        bool is_walkable;
    };

    GameMap(
        int width,
        int height,
        int padding_x,
        int padding_y,
        std::size_t cell_size);

    void Init();
    void Update();
    void Render(SDL_Renderer& renderer);

    void SetIsWalkable(int row, int col, bool is_walkable);
    bool AreRowColWalkable(int row, int col) const;
    bool IsWalkable(std::size_t index) const;
    bool AreCoordsWalkable(int x, int y) const;

    std::size_t FromRowColToIndex(int row, int col) const;
    using CoordsPair = std::pair<int, int>;
    CoordsPair FromRowColToCoords(int row, int col) const;

    using RowColPair = std::pair<int, int>;
    RowColPair FromIndexToRowCol(std::size_t index) const;
    RowColPair FromCoordsToRowCol(int coord_x, int coord_y) const;
    RowColPair GetCloserWalkableRowCol(Vec2 row_col, int cells_distance, Vec2 direction) const;

    std::size_t GetRowsCount() const;
    std::size_t GetColumnsCount() const;
    std::size_t GetCellsCount() const;
    std::size_t GetCellSize() const;
    int GetCellSizeInt() const;

    bool AreRowColInsideBoundaries(int row, int col) const;
    bool AreCoordsInsideBoundaries(int x, int y) const;

    const Cell& GetCell(int row, int col) const;
    const std::vector<Cell>& GetCells() const;

private:
    int width_;
    int height_;
    int padding_x_;
    int padding_y_;
    std::size_t cells_size_;
    int cell_size_int_;
    std::size_t rows_count_;
    std::size_t cols_count_;
    std::size_t cells_count_;

    std::vector<Cell> cells_;

    bool IsInsideBoundaries(std::size_t index) const;
};
