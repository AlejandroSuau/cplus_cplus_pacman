#pragma once

#include <SDL2/SDL.h>

#include "utils/Vec2.hpp"

#include <string>
#include <array>

// Update
static const Uint64 kTargetFPS = 60;
static const Uint64 kFixedTimeStep = 1000 / kTargetFPS;
static const Uint64 kFrameDelay = 1000 / kTargetFPS; // Frame delay in milliseconds

// Game
static const std::string kAssetsFolderImages = "assets/images/";
static const std::string kAssetsFolderFonts = "assets/fonts/";
static const std::string kAssetsFolderSounds = "assets/sounds/";

static const std::size_t kCellSize = 16 * 2;
static const int kCellSizeInt = static_cast<int>(kCellSize);
static const int kGamePaddingX = 100;
static const int kGamePaddingY = 100;
static const std::size_t kColsCount = 17;
static const std::size_t kRowsCount = 20;
static const int kGameWidth = kCellSizeInt * static_cast<int>(kColsCount); // 80 cols
static const int kGameHeight = kCellSizeInt * static_cast<int>(kRowsCount); // 60 rows

static const std::array<std::array<unsigned int, kColsCount>, kRowsCount> kMapTiles {{
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0},
    {1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1},
    {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0},
    {0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0},
    {0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0},
    {0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0},
    {0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0},
    {1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0},
    {0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1},
    {0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
}};

static const SDL_Rect kTextureRectBackground {-16, -16, kGameWidth + kCellSizeInt, kGameHeight + kCellSize};

// Score
namespace ScoreParamenters {
static const Vec2 kTextureDimensions {24, 36};
static const Vec2 kPosition {0, 25};
}

// Sprite Sheet
namespace SpriteSheet {
static const int kPadding = 6;
static const int kWidth = 14;
static const int kHeight = kWidth;
static const int kStartingX = 3;
static const int kStartingY = 3;

namespace GhostSprite {
static const int kStartingY = 83;
static const int kAnimationCount = 2;
}

}

// Ghost
namespace GhostParameters {
static const float kVelocityStateHousing {150.f};
static const float kVelocityStateChasing {100.f};
}

// Player
namespace PlayerParameters {
static const unsigned int kLifes = 3;
static const float kVelocity = 125.f;
static const int kWidth = 31;
static const int kHeight = kWidth;
static const int kMovingAnimationCount = 3;
static const int kDyingAnimationCount = 11;
}
