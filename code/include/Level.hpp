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
    Level();

    void IncreaseLevel();
    void SetNumber(unsigned int number);
    
    unsigned int GetNumber() const;
    float GetSpeedGhost() const;
    float GetSpeedGhostFrightened() const;
    float GetSpeedPlayer() const;
    float GetSecondsDurationGhostFrightened() const;

private:
    static const float kMaxSpeed;

    unsigned int number_;
    float speed_percentage_player_;
    float speed_percentage_ghost_;
    float speed_percentage_ghost_frightened_;
    float seconds_duration_ghost_frightened_;
};
