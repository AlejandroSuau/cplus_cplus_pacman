#pragma once

#include <functional>

class CountdownTimer {
public:
    CountdownTimer(float interval_seconds);

    void Restart();
    void Update(float dt);
    bool DidFinish() const;

    void SetIntervalSeconds(float interval_seconds);
    float GetElapsedSeconds() const;
    float GetIntervalSeconds() const;
    float GetSecondsToFinish() const;

    using OnFinishCallback = std::function<void()>;
    void SetOnFinishCallback(OnFinishCallback callback);

private:
    float interval_seconds_;
    float elapsed_seconds_;
    bool did_finish_;

    OnFinishCallback on_finish_callback_;
};