#include "Timer.h"
#include <algorithm>


// Блок 1
// Задати, запустити, скинути
void Timer::set_duration(int secs) {
    total = std::chrono::seconds(secs);
    state = State::Idle;
}

void Timer::start() {
    if (is_running()) return;                // захист від подвійного старту
    deadline = Clock::now() + total;
    state = State::Running;
}

void Timer::reset() {
    state = State::Idle;
}

// Блок 2
// Пауза й продовження
void Timer::pause() {
    if (state != State::Running) return;
    paused_remaining = std::max(deadline - Clock::now(), Clock::duration::zero());
    state = State::Paused;
}

void Timer::resume() {
    if (state != State::Paused) return;
    deadline = Clock::now() + paused_remaining;
    state = State::Running;
}

// Блок 3
// Запитати стан
bool Timer::is_running() const {
    return state == State::Running && Clock::now() < deadline;
}

int Timer::remaining_seconds() const {
    Clock::duration left{};

    switch (state) {
        case State::Idle:    left = total;                   break;
        case State::Paused:  left = paused_remaining;        break;
        case State::Running: left = deadline - Clock::now(); break;
    }

    if (left < Clock::duration::zero()) left = Clock::duration::zero();

    return static_cast<int>(std::chrono::ceil<std::chrono::seconds>(left).count());
}