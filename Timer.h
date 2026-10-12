//
// Created by Кирило Вєтров on 12.10.2026.
//

#ifndef TIMER_TIMER_H
#define TIMER_TIMER_H

#include <atomic>

class Timer {
private:
    std::atomic<bool> running{false};
    std::atomic<bool> paused{false};
    int total_seconds{0};
    int remaining_seconds{0};

    void print_time() const;

public:
    explicit Timer(int seconds);

    void start();
    void pause();
    void resume();
    void stop();
    void reset();

    int get_remaining() const;
};

#endif //TIMER_TIMER_H
