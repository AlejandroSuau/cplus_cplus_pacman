#pragma once

#include <array>

#include "Player.hpp"
#include "Ghost.hpp"

class CollisionManager {
public:

private:
    Player& player_;
    std::array<Ghost, 4>& ghosts_;
};
