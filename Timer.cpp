//
// Created by Кирило Вєтров on 12.10.2026.
//

#include "Timer.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>

Timer::Timer(int seconds) : total_seconds(seconds), remaining_seconds(seconds) {}

void Timer::start() {
    running = true;
    paused = false;

    while (running && remaining_seconds > 0) {
        if (!paused) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            remaining_seconds--;
            print_time();
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    if (remaining_seconds == 0 && running) {
        std::cout << "\n[Таймер завершено!]\n";
    }
    running = false;
}

void Timer::pause() {
    paused = true;
}

void Timer::resume() {
    paused = false;
}

void Timer::stop() {
    running = false;
}

void Timer::reset() {
    running = false;
    paused = false;
    remaining_seconds = total_seconds;
}

int Timer::get_remaining() const {
    return remaining_seconds;
}

void Timer::print_time() const {
    int minutes = remaining_seconds / 60;
    int seconds = remaining_seconds % 60;

    std::cout << "\rЗалишилось часу: "
              << std::setfill('0') << std::setw(2) << minutes << ":"
              << std::setfill('0') << std::setw(2) << seconds
              << "   " << std::flush;
}