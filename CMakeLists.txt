cmake_minimum_required(VERSION 3.28)
project(TimerProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(TimerProject
        main.cpp
        TimerWindow.h
        TimerWindow.cpp
        Timer.h
        Timer.cpp
        resources.qrc   # <-- Додали файл ресурсів
        styles.qss
        SettingsWindow.cpp
        SettingsWindow.h
)

target_link_libraries(TimerProject PRIVATE Qt6::Widgets)