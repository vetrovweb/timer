cmake_minimum_required(VERSION 3.22)
project(TimerProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Вказуємо Homebrew-шлях до Qt6, якщо CMake його не знаходить автоматично
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt")

# Шукаємо компоненти Qt 6 (Widgets відповідає за графічні елементи: вікна, кнопки, тексти)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Увімкнення автоматичної обробки мета-об'єктної системи Qt (MOC, UIC, RCC)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Наш виконуваний файл та вихідні файли
add_executable(TimerProject
        main.cpp
        Timer.h
        Timer.cpp
        TimerWindow.h
        TimerWindow.cpp
)

# Підключаємо бібліотеку Qt Widgets до нашого проєкту
target_link_libraries(TimerProject PRIVATE Qt6::Widgets)