#include "TimerWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QStackedLayout>

TimerWindow::TimerWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("C++20 Timer");
    resize(480, 380);

    // Встановлюємо білий фон для головного вікна
    this->setStyleSheet("background-color: #ffffff;");

    // Головний вертикальний лейаут вікна
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 40, 40, 40);
    mainLayout->setAlignment(Qt::AlignCenter);

    // 1. Циферблат часу
    timeLabel = new QLabel("00:00", this);
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->setStyleSheet("font-size: 102px; font-weight: 500; color: #17325c; background: transparent;");
    mainLayout->addWidget(timeLabel);

    mainLayout->addSpacing(25);

    // 2. QStackedLayout для перемикання наборів кнопок
    stackedLayout = new QStackedLayout();
    mainLayout->addLayout(stackedLayout);

    // --- СТОРІНКА 0: Налаштування ---
    setupWidget = new QWidget(this);
    setupWidget->setStyleSheet("background: transparent;");
    QVBoxLayout *setupLayout = new QVBoxLayout(setupWidget);
    setupLayout->setContentsMargins(0, 0, 0, 0);
    setupLayout->setAlignment(Qt::AlignCenter);

    QHBoxLayout *addButtonsLayout = new QHBoxLayout();
    addButtonsLayout->setAlignment(Qt::AlignCenter);
    addButtonsLayout->setSpacing(12);

    btnPlus30 = new QPushButton("+0:30", this);
    btnPlus1Min = new QPushButton("+1:00", this);
    btnPlus5Min = new QPushButton("+5:00", this);

    QString addBtnStyle =
        "QPushButton {"
        "   background-color: #ffffff;"
        "   color: #1a73e8;"
        "   border: 1px solid #1a73e8;"
        "   border-radius: 18px;"
        "   padding: 8px 20px;"
        "   min-height: 20px;"
        "   font-size: 14px;"
        "   font-weight: 500;"
        "}"
        "QPushButton:hover {"
        "   background-color: #f6f8fc;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #e8f0fe;"
        "}";

    btnPlus30->setStyleSheet(addBtnStyle);
    btnPlus1Min->setStyleSheet(addBtnStyle);
    btnPlus5Min->setStyleSheet(addBtnStyle);

    addButtonsLayout->addWidget(btnPlus30);
    addButtonsLayout->addWidget(btnPlus1Min);
    addButtonsLayout->addWidget(btnPlus5Min);
    setupLayout->addLayout(addButtonsLayout);

    setupLayout->addSpacing(35);

    startBtn = new QPushButton("▶", this);
    startBtn->setFixedHeight(56);
    startBtn->setFixedWidth(160);
    startBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #ffffff;"
        "   color: #1a73e8;"
        "   border: 1px solid #1a73e8;"
        "   border-radius: 28px;"
        "   font-size: 20px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #f6f8fc;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #e8f0fe;"
        "}"
        "QPushButton:disabled {"
        "   border: 1px solid #dadce0;"
        "   color: #dadce0;"
        "   background-color: #ffffff;"
        "}"
    );
    startBtn->setEnabled(false);
    setupLayout->addWidget(startBtn, 0, Qt::AlignCenter);

    stackedLayout->addWidget(setupWidget);

    // --- СТОРІНКА 1: Активний таймер ---
    runningWidget = new QWidget(this);
    runningWidget->setStyleSheet("background: transparent;");
    QHBoxLayout *runningLayout = new QHBoxLayout(runningWidget);
    runningLayout->setContentsMargins(0, 0, 0, 0);
    runningLayout->setAlignment(Qt::AlignCenter);
    runningLayout->setSpacing(20);

    pauseBtn = new QPushButton("⏸", this);
    resetBtn = new QPushButton("↺", this);

    QString controlBtnStyle =
        "QPushButton {"
        "   background-color: #ffffff;"
        "   color: #1a73e8;"
        "   border: 1px solid #1a73e8;"
        "   border-radius: 28px;"
        "   font-size: 22px;"
        "   min-width: 110px;"
        "   min-height: 56px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #f6f8fc;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #e8f0fe;"
        "}";

    pauseBtn->setStyleSheet(controlBtnStyle);
    resetBtn->setStyleSheet(controlBtnStyle);

    runningLayout->addWidget(pauseBtn);
    runningLayout->addWidget(resetBtn);

    stackedLayout->addWidget(runningWidget);

    stackedLayout->setCurrentIndex(0);

    // Таймер оновлення з максимально точним реагуванням
    uiTimer = new QTimer(this);
    uiTimer->setInterval(1000);

    // Підключення сигналів і слотів
    connect(btnPlus30, &QPushButton::clicked, this, &TimerWindow::add30Seconds);
    connect(btnPlus1Min, &QPushButton::clicked, this, &TimerWindow::add1Minute);
    connect(btnPlus5Min, &QPushButton::clicked, this, &TimerWindow::add5Minutes);

    connect(startBtn, &QPushButton::clicked, this, &TimerWindow::onStartClicked);
    connect(pauseBtn, &QPushButton::clicked, this, &TimerWindow::onPauseClicked);
    connect(resetBtn, &QPushButton::clicked, this, &TimerWindow::onResetClicked);
    connect(uiTimer, &QTimer::timeout, this, &TimerWindow::updateGui);

    updateGuiDisplay();
}

void TimerWindow::add30Seconds() {
    currentSeconds += 30;
    updateGuiDisplay();
}

void TimerWindow::add1Minute() {
    currentSeconds += 60;
    updateGuiDisplay();
}

void TimerWindow::add5Minutes() {
    currentSeconds += 300;
    updateGuiDisplay();
}

void TimerWindow::onStartClicked() {
    if (currentSeconds <= 0) return;

    timer.set_duration(currentSeconds);
    timer.start();

    stackedLayout->setCurrentIndex(1);
    pauseBtn->setText("⏸");

    // Миттєве оновлення екрану без очікування першого тіка таймера
    updateGui();
    uiTimer->start();
}

void TimerWindow::onPauseClicked() {
    if (timer.is_running()) {
        timer.pause();
        pauseBtn->setText("▶");
    } else {
        timer.resume();
        pauseBtn->setText("⏸");
    }
}

void TimerWindow::onResetClicked() {
    timer.reset();
    uiTimer->stop();

    currentSeconds = 0;
    switchToSetupMode();
    updateGuiDisplay();
}

void TimerWindow::updateGui() {
    auto remaining = timer.remaining_seconds();

    if (remaining <= 0 && !timer.is_running()) {
        uiTimer->stop();
        timeLabel->setText("Час вийшов!");
        switchToSetupMode();
        currentSeconds = 0;
        updateGuiDisplay();
        return;
    }

    int minutes = remaining / 60;
    int secs = remaining % 60;

    QString timeStr = QString("%1:%2")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(secs, 2, 10, QChar('0'));

    timeLabel->setText(timeStr);
}

void TimerWindow::updateGuiDisplay() {
    int minutes = currentSeconds / 60;
    int secs = currentSeconds % 60;

    QString timeStr = QString("%1:%2")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(secs, 2, 10, QChar('0'));

    timeLabel->setText(timeStr);
    startBtn->setEnabled(currentSeconds > 0);
}

void TimerWindow::switchToSetupMode() {
    stackedLayout->setCurrentIndex(0);
}