#pragma once

#include <atomic>
#include <thread>

class Timer {
public:
    Timer();
    ~Timer();

    void set_duration(int seconds);
    void start();
    void pause();
    void resume();
    void reset();

    bool is_running() const;
    int remaining_seconds() const;

private:
    void countdown_loop();

    int total_seconds{0};
    std::atomic<int> current_remaining{0};
    std::atomic<bool> running{false};
    std::atomic<bool> paused{false};
    std::thread worker_thread;
};