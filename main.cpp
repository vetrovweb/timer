#include <iostream>
#include <thread>
#include "Timer.h"

void print_help() {
    std::cout << "\nКоманди керування:\n"
              << "  [p] - поставити на паузу\n"
              << "  [r] - відновити роботу\n"
              << "  [q] - зупинити та вийти\n"
              << "Введіть команду: ";
}

int main() {
    std::cout << "=== Інтерактивний Таймер на C++20 ===\n";

    int seconds = 0;
    std::cout << "Введіть час таймера у секундах: ";
    if (!(std::cin >> seconds) || seconds <= 0) {
        std::cout << "Будь ласка, введіть коректне додатне число.\n";
        return 1;
    }

    Timer my_timer(seconds);

    // Запускаємо таймер в окремому потоці
    std::thread timer_thread(&Timer::start, &my_timer);

    char command;
    print_help();

    // Головний потік слухає команди користувача
    while (std::cin >> command) {
        switch (command) {
            case 'p':
            case 'P':
                my_timer.pause();
                std::cout << "[Таймер на паузі]\n";
                break;
            case 'r':
            case 'R':
                my_timer.resume();
                std::cout << "[Таймер відновлено]\n";
                break;
            case 'q':
            case 'Q':
                my_timer.stop();
                std::cout << "[Вихід з програми]\n";
                goto end_loop; // Виходимо з циклу обробки команд
            default:
                std::cout << "Невідома команда. Використовуйте p, r або q.\n";
                break;
        }

        // Перевіряємо, чи таймер випадково не завершився сам
        if (my_timer.get_remaining() == 0) {
            break;
        }
    }

    end_loop:
        // Очікуємо завершення потоку таймера
        if (timer_thread.joinable()) {
            timer_thread.join();
        }

    std::cout << "Роботу програми завершено.\n";
    return 0;
}