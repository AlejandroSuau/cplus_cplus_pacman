#pragma once

#include "utils/Vec2.hpp"

#include <string>
#include <vector>
#include <array>
#include <set>

#include <SDL2/SDL.h>

class GameMap;

class Pathfinder {
    struct MapNode {
            std::size_t map_index;
            int g; // distance from starting_node
            int h; // heuristic (distance from target node)
            bool is_open, is_closed;
            MapNode* parent;

            MapNode(std::size_t map_idx)
                : map_index(map_idx), g(0), h(0), is_open(false), is_closed(false), parent(nullptr) {}

            int FCost() const { return g + h; }

            void Print() {
                SDL_Log("MapNode {map_index: %zu, g: %d, h: %d}", map_index, g, h);
            }

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
    using Path = std::vector<Vec2<int>>;
    Pathfinder(GameMap& map);

    Path FindPath(
        Vec2<int> col_row_from,
        Vec2<int> col_row_to);

    void Step();
    void Render(SDL_Renderer& renderer);
    void Reset(Vec2<int> col_row_from = {}, Vec2<int> col_row_to = {});
    bool DidFinish() const;

private:
    GameMap& map_;
    std::vector<MapNode> map_nodes_;
    std::set<MapNode*, MapNode::Comparator> open_nodes_;
    std::size_t target_nodes_count_;

    std::size_t target_index_;
    Pathfinder::MapNode* target_node_;

    using Neighbours = std::array<MapNode*, 4>;
    Neighbours GetNeighbours(std::size_t node_index);

    Vec2<int> col_row_from_;
    Vec2<int> col_row_to_;
    bool did_finish_;

    int Heuristic(Vec2<int> col_row_left, Vec2<int> col_row_right) const;
    Path ReconstructPath() const;
};
