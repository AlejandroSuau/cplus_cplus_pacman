#include "utils/CountdownTimer.hpp"

CountdownTimer::CountdownTimer(float interval_seconds)
    : interval_seconds_(interval_seconds)
    , elapsed_seconds_(0.f)
    , did_finish_(false)
    , on_finish_callback_(nullptr) {}

void CountdownTimer::Restart() {
    elapsed_seconds_ = 0.f;
}

void CountdownTimer::Update(float dt) {
    did_finish_ = false;
    elapsed_seconds_ += dt;
    
    if (elapsed_seconds_ >= interval_seconds_) {
        did_finish_ = true;
        elapsed_seconds_ = 0;
        if (on_finish_callback_) {
            on_finish_callback_();
        }
    }
}

void CountdownTimer::SetOnFinishCallback(OnFinishCallback callback) {
    on_finish_callback_ = std::move(callback);
}

void CountdownTimer::SetIntervalSeconds(float interval_seconds) {
    interval_seconds_ = interval_seconds;
    Restart();
}

float CountdownTimer::GetSecondsToFinish() const {
    return (interval_seconds_ - elapsed_seconds_);
}

bool CountdownTimer::DidFinish() const {
    return did_finish_;
}

float CountdownTimer::GetIntervalSeconds() const {
    return interval_seconds_;
}

float CountdownTimer::GetElapsedSeconds() const {
    return elapsed_seconds_;
}
