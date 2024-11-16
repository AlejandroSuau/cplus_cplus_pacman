#pragma once

class CountdownTimer {
public:
    CountdownTimer(float interval_seconds);

    void Restart();
    void Update(float dt);
    bool DidFinish() const;

private:
    float interval_seconds_;
    float elapsed_seconds_;
    bool did_finish_;
};