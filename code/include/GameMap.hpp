#pragma once

#include <vector>
#include <utility>

#include <SDL2/SDL.h>

class GameMap {
public:
    GameMap(int width, int height, std::size_t cell_size);

    void Init();
    void Update();
    void Render(SDL_Renderer& renderer);

    void Occupy(int row, int col);
    void Unoccupy(int row, int col);

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

    std::size_t GetRowsCount() const;
    std::size_t GetColumnsCount() const;
    std::size_t GetCellsCount() const;

    bool AreRowColInsideBoundaries(int row, int col) const;
    bool AreCoordsInsideBoundaries(int x, int y) const;

private:
    struct Cell {
        Cell(std::size_t cell_index, int x, int y)
            : cell_index_(cell_index), x_(x), y_(y), is_occupied_(false), is_walkable_(true) {}
        
        std::size_t cell_index_;
        int row_, col_;
        int x_, y_;
        bool is_occupied_;
        bool is_walkable_;
    };

    int width_;
    int height_;
    std::size_t cells_size_;
    int cell_size_int_;
    std::size_t rows_count_;
    std::size_t cols_count_;
    std::size_t cells_count_;

    std::vector<Cell> cells_;

    void SetOccupy(int row, int col, bool is_occupied);
    bool IsInsideBoundaries(std::size_t index) const;
};
