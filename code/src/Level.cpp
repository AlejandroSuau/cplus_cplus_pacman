#include "Level.hpp"

#include <cmath>

const float Level::kMaxSpeed = 120.f;

Level::Level() {
    SetNumber(1);
}

void Level::IncreaseLevel() {
    SetNumber(++number_);
}

void Level::SetNumber(unsigned int number) {
    number_ = number;
    
    const auto number_float = static_cast<float>(number);
    speed_percentage_player_ = std::min(80.f + number_float, 100.f) / 100.f;
    speed_percentage_ghost_ = std::min(75.f + number_float, 110.f) / 100.f;
    speed_percentage_ghost_frightened_ = std::min(50.f + number_float / 2.f, 70.f) / 100.f;
    seconds_duration_ghost_frightened_ = std::max(6.f - number_float / 10.f, 0.5f); 
}

unsigned int Level::GetNumber() const {
    return number_;
}

float Level::GetSpeedGhost() const {
    return kMaxSpeed * speed_percentage_ghost_;
}

float Level::GetSpeedGhostFrightened() const {
    return kMaxSpeed * speed_percentage_ghost_frightened_;
}

float Level::GetSpeedPlayer() const {
    return kMaxSpeed * speed_percentage_player_;
}

float Level::GetSecondsDurationGhostFrightened() const {
    return seconds_duration_ghost_frightened_;
}
