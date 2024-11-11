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

static const int kGameWidth = 720;
static const int kGameHeight = 1080;
static const std::size_t kCellSize = 16;
static const int kCellSizeInt = static_cast<int>(kCellSize);

static const SDL_FRect kTextureRectBackground {0.f, 0.f, kGameWidth, kGameHeight};

// Score
namespace ScoreParamenters {
static const Vec2 kTextureDimensions {24.f, 36.f};
static const Vec2 kPosition {0.f, 25.f};
}
