#pragma once

#include <cmath>

/*  
    Level   PacmanVel(%)  GhostFrightVel(%)     
    
    (%) Pacman Velocity: 80% and increases 1% each level until 100%
    (%) Ghost Velocity: 75% and increases 1% each level until 110%
    (%) Ghost Frightened Velocity: 50% and increases level/2 each level until 70.f
    (seconds) Ghost Frightened Duration: Starts 5.9 and decreases level/10 each level until 0.5
*/

class Level {
public:
    Level() {
        SetNumber(1);
    }

    void IncreaseLevel() {
        SetNumber(++number_);
    }

    void SetNumber(unsigned int number) {
        number_ = number;
        
        const auto number_float = static_cast<float>(number);
        speed_percentage_player_ = std::min(80.f + number_float, 100.f);
        speed_percentage_ghost_ = std::min(75.f + number_float, 110.f);
        speed_percentage_ghost_frightened_ = std::min(50.f + number_float / 2.f, 70.f);
        seconds_duration_ghost_frightened_ = std::max(6.f - number_float / 10.f, 0.5f); 
    }

    unsigned int GetNumber() const {
        return number_;
    }

private:
    unsigned int number_;
    float speed_percentage_player_;
    float speed_percentage_ghost_;
    float speed_percentage_ghost_frightened_;
    float seconds_duration_ghost_frightened_;
};
