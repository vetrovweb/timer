#pragma once

#include <atomic>
//#include <thread>

class Timer {
public:
    void set_duration(int seconds);
    void start();
    void pause();
    void resume();
    void reset();

    bool is_running() const;
    int remaining_seconds() const;

private:
    //void countdown_loop();

    //int total_seconds{0};
    // std::atomic<int> current_remaining{0};
    // std::atomic<bool> running{false};
    // std::atomic<bool> paused{false};
    // std::thread worker_thread;

    // steady_clock - рахує час з якоїсь точки, a
    // system_clock - системний час, який може змінюватись (з літнього на зимовий)
    using Clock = std::chrono::steady_clock;

    enum class State { Idle, Running, Paused };

    State state{State::Idle};
    Clock::duration total{};
    Clock::duration paused_remaining{};
    Clock::time_point deadline{};
};