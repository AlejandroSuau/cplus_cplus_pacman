#pragma once

#include "pathfinder/Pathfinder.hpp"

#include "utils/Vec2.hpp"

#include <functional>

class Game;

using PathfindingPattern = std::function<Pathfinder::Path(Vec2, Game&)>;

Pathfinder::Path FindPathPatternInky(const Vec2& ghost_row_col, Game& game);
Pathfinder::Path FindPathPatternBlinky(const Vec2& ghost_row_col, Game& game);
Pathfinder::Path FindPathPatternPinky(const Vec2& ghost_row_col, Game& game);
Pathfinder::Path FindPathPatternClyde(const Vec2& ghost_row_col, Game& game);
