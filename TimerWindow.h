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

public slots:
    void setWindowTransparency(int opacityPercent);
    void setLockOpacity(bool locked);
    void setCustomTextColor(const QString &colorHex);
    void setCustomBgColor(const QString &colorHex);

private slots:
    void add30Seconds();
    void add1Minute();
    void add5Minutes();

    void onStartClicked();
    void onPauseClicked();
    void onResetClicked();
    void updateGui();

    void onMenuClicked();

private:
    void switchToSetupMode();
    void updateGuiDisplay();
    void applyTextOpacity();
    void updateStyles();

    Timer timer;
    int currentSeconds{0};

    int currentOpacityPercent{100};
    bool isOpacityLocked{true};
    QString currentTextColor{"#17325c"};
    QString currentBgColor{"#ffffff"};

    QPushButton *menuBtn;

    // Роздільні елементи циферблата для ідеального центрування двокрапки
    QLabel *minutesLabel;
    QLabel *colonLabel;
    QLabel *secondsLabel;

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