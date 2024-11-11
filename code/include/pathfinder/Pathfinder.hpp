#pragma once

#include <string>
#include <vector>
#include <array>
#include <set>

#include <SDL2/SDL.h>

class GameMap;

class Pathfinder {
private:
    struct MapNode {
            std::size_t map_index;
            int g; // distance from starting_node
            int h; // heuristic (distance from target node)
            bool is_open, is_closed;
            MapNode* parent;

            MapNode(std::size_t map_idx)
                : map_index(map_idx), g(0), h(0), is_open(false), is_closed(false), parent(nullptr) {}

            int FCost() const { return g + h; }

            struct Comparator {
                int operator()(const MapNode* n1, const MapNode* n2) const {
                    if (n1->FCost() == n2->FCost()) {
                        return n1->map_index < n2->map_index;
                    }

                    return (n1->FCost() < n2->FCost());
                }
            };
        };
public:
    using Path = std::vector<std::pair<int, int>>;

    Pathfinder(GameMap& map);

    Path FindPath(
        int start_row,
        int start_col,
        int target_row,
        int target_col);
    void Step();
    void Render(SDL_Renderer& renderer);
    void Reset(int start_row, int start_col, int target_row, int target_col);
    bool DidFinish() const;

private:
    GameMap& map_;
    std::vector<MapNode> map_nodes_;
    std::set<MapNode*, MapNode::Comparator> open_nodes_;
    std::size_t solution_nodes_count_;

    std::size_t target_index_;
    Pathfinder::MapNode* target_node_;

    using Neighbours = std::array<MapNode*, 4>;
    Neighbours GetNeighbours(std::size_t node_index);

    /////////
    int start_row_, start_col_;
    int target_row_, target_col_;
    bool did_finish_;

    int Heuristic(int row1, int col1, int row2, int col2) const;
    Path ReconstructPath() const;
};
