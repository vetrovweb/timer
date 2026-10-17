#pragma once

#include <chrono>

class Timer {
public:
    void set_duration(int secs);
    void start();
    void pause();
    void resume();
    void reset();

    bool is_running() const;
    int remaining_seconds() const;

private:
    using Clock = std::chrono::steady_clock;        // краще за system_clock (він рахує системний час)
    enum class State { Idle, Running, Paused };

    State state{State::Idle};
    Clock::duration total{};                        // скільки задали
    Clock::duration paused_remaining{};             // залишок, збережений на паузі
    Clock::time_point deadline{};                   // момент закінчення (коли таймер іде)
};