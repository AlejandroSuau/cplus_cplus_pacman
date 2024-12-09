#pragma once

#include "pathfinder/Pathfinder.hpp"

#include "utils/Vec2.hpp"

#include <functional>

class GameScene;

using PathfindingPattern = std::function<Pathfinder::Path(Vec2<int>, GameScene&)>;

Pathfinder::Path FindPathPatternInky(const Vec2<int>& col_row_ghost, GameScene& game);
Pathfinder::Path FindPathPatternBlinky(const Vec2<int>& col_row_ghost, GameScene& game);
Pathfinder::Path FindPathPatternPinky(const Vec2<int>& col_row_ghost, GameScene& game);
Pathfinder::Path FindPathPatternClyde(const Vec2<int>& col_row_ghost, GameScene& game);
