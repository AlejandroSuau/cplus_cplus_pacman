#pragma once

#include <SDL2/SDL.h>

#include "utils/Vec2.hpp"

#include <string>

// Update
static const Uint64 kTargetFPS = 60;
static const Uint64 kFixedTimeStep = 1000 / kTargetFPS;
static const Uint64 kFrameDelay = 1000 / kTargetFPS; // Frame delay in milliseconds

// Game
static const std::string kAssetsFolderImages = "assets/images/";
static const std::string kAssetsFolderSounds = "assets/sounds/";

static const std::size_t kCellSize = 16 * 2;
static const int kCellSizeInt = static_cast<int>(kCellSize);
static const int kGameWidth = kCellSizeInt * 40; // 80 cols
static const int kGameHeight = kCellSizeInt * 30; // 60 rows

static const SDL_FRect kTextureRectBackground {0.f, 0.f, kGameWidth, kGameHeight};

// Score
namespace ScoreParamenters {
static const Vec2 kTextureDimensions {24.f, 36.f};
static const Vec2 kPosition {0.f, 25.f};
}

// Sprite Sheet
namespace SpriteSheet {

namespace GhostSprite {
static const int kStartingX = 3;
static const int kStartingY = 83;
static const int kPadding = 6;
static const int kWidth = 14;
static const int kHeight = kWidth;
static const int kAnimationCount = 2;
}

}