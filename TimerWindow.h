#pragma once

#include <QWidget>
#include "Timer.h"

class QLabel;
class QPushButton;
class QTimer;
class QStackedLayout;

class TimerWindow : public QWidget {
    Q_OBJECT

public:
    TimerWindow(QWidget *parent = nullptr);
    ~TimerWindow() = default;

private slots:
    void add30Seconds();
    void add1Minute();
    void add5Minutes();

    void onStartClicked();
    void onPauseClicked();
    void onResetClicked();
    void updateGui();

private:
    void switchToSetupMode();
    void updateGuiDisplay();

    Timer timer;
    int currentSeconds{0};

    QLabel *timeLabel;

    QWidget *setupWidget;
    QPushButton *btnPlus30;
    QPushButton *btnPlus1Min;
    QPushButton *btnPlus5Min;
    QPushButton *startBtn;

    QWidget *runningWidget;
    QPushButton *pauseBtn;
    QPushButton *resetBtn;

    QStackedLayout *stackedLayout;
    QTimer *uiTimer;
};