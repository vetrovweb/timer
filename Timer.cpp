#include "Timer.h"
#include <chrono>

Timer::Timer() = default;

Timer::~Timer() {
    reset();
}

void Timer::set_duration(int seconds) {
    total_seconds = seconds;
    current_remaining = seconds;
}

void Timer::start() {
    if (running) return;

    running = true;
    paused = false;

    if (worker_thread.joinable()) {
        worker_thread.join();
    }

    worker_thread = std::thread(&Timer::countdown_loop, this);
}

void Timer::pause() {
    paused = true;
}

void Timer::resume() {
    paused = false;
}

void Timer::reset() {
    running = false;
    paused = false;

    if (worker_thread.joinable()) {
        worker_thread.join();
    }

    current_remaining = total_seconds;
}

bool Timer::is_running() const {
    return running && !paused;
}

int Timer::remaining_seconds() const {
    return current_remaining.load();
}

void Timer::countdown_loop() {
    while (running && current_remaining > 0) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        if (running && !paused) {
            if (current_remaining > 0) {
                current_remaining--;
            }
        }
    }
    running = false;
}