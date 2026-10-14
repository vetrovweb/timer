#include "TimerWindow.h"
#include "SettingsWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QStackedLayout>
#include <QFile>
#include <QTextStream>
#include <QColor>
#include <QStyle>
#include <QSizePolicy>

TimerWindow::TimerWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("C++20 Timer");
    resize(480, 380);

    updateStyles();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 12, 20, 40);

    // --- ШАПКА ВІКНА: Кнопка меню справа ---
    QHBoxLayout *topBarLayout = new QHBoxLayout();
    topBarLayout->setContentsMargins(0, 0, 0, 0);
    topBarLayout->addStretch();

    menuBtn = new QPushButton("☰", this);
    menuBtn->setObjectName("menuBtn");
    topBarLayout->addWidget(menuBtn);

    mainLayout->addLayout(topBarLayout);
    mainLayout->addSpacing(15);

    // --- 1. ЦИФЕРБЛАТ (Симетричні рівні блоки з фіксованим центром) ---
    QHBoxLayout *timerDisplayLayout = new QHBoxLayout();
    timerDisplayLayout->setContentsMargins(0, 0, 0, 0);
    timerDisplayLayout->setSpacing(0);
    timerDisplayLayout->setAlignment(Qt::AlignCenter);

    minutesLabel = new QLabel("00", this);
    minutesLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    minutesLabel->setObjectName("timerDigitLabel");
    minutesLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    colonLabel = new QLabel(":", this);
    colonLabel->setAlignment(Qt::AlignCenter);
    colonLabel->setObjectName("timerColonLabel");
    colonLabel->setFixedWidth(24); // Жорсткий центр для двокрапки

    secondsLabel = new QLabel("00", this);
    secondsLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    secondsLabel->setObjectName("timerDigitLabel");
    secondsLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    timerDisplayLayout->addWidget(minutesLabel, 1); // Рівна вага зліва
    timerDisplayLayout->addWidget(colonLabel, 0);  // Центральний елемент
    timerDisplayLayout->addWidget(secondsLabel, 1); // Рівна вага справа

    mainLayout->addLayout(timerDisplayLayout);

    mainLayout->addSpacing(25);

    // --- 2. QStackedLayout для перемикання наборів кнопок ---
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

    addButtonsLayout->addWidget(btnPlus30);
    addButtonsLayout->addWidget(btnPlus1Min);
    addButtonsLayout->addWidget(btnPlus5Min);
    setupLayout->addLayout(addButtonsLayout);

    setupLayout->addSpacing(35);

    startBtn = new QPushButton("▶", this);
    startBtn->setObjectName("startBtn");
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
    pauseBtn->setObjectName("pauseBtn");

    resetBtn = new QPushButton("↺", this);
    resetBtn->setObjectName("resetBtn");

    runningLayout->addWidget(pauseBtn);
    runningLayout->addWidget(resetBtn);

    stackedLayout->addWidget(runningWidget);

    stackedLayout->setCurrentIndex(0);

    // Таймер оновлення
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

    connect(menuBtn, &QPushButton::clicked, this, &TimerWindow::onMenuClicked);

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

void TimerWindow::onMenuClicked() {
    SettingsWindow settingsDlg(currentOpacityPercent, isOpacityLocked, currentTextColor, currentBgColor, this);

    connect(&settingsDlg, &SettingsWindow::opacityChanged, this, &TimerWindow::setWindowTransparency);
    connect(&settingsDlg, &SettingsWindow::lockOpacityToggled, this, &TimerWindow::setLockOpacity);
    connect(&settingsDlg, &SettingsWindow::textColorChanged, this, &TimerWindow::setCustomTextColor);
    connect(&settingsDlg, &SettingsWindow::bgColorChanged, this, &TimerWindow::setCustomBgColor);

    settingsDlg.exec();
}

void TimerWindow::setWindowTransparency(int opacityPercent) {
    currentOpacityPercent = opacityPercent;
    qreal opacity = static_cast<qreal>(opacityPercent) / 100.0;
    setWindowOpacity(opacity);
    applyTextOpacity();
}

void TimerWindow::setLockOpacity(bool locked) {
    isOpacityLocked = locked;
    applyTextOpacity();
}

void TimerWindow::setCustomTextColor(const QString &colorHex) {
    currentTextColor = colorHex;
    updateStyles();
    applyTextOpacity();
}

void TimerWindow::setCustomBgColor(const QString &colorHex) {
    currentBgColor = colorHex;
    updateStyles();
}

void TimerWindow::updateStyles() {
    QFile styleFile(":/styles.qss");
    QString baseStyles = "";
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&styleFile);
        baseStyles = stream.readAll();
        styleFile.close();
    }

    QColor bgCol(currentBgColor);
    bool isDarkBg = (bgCol.red() * 0.299 + bgCol.green() * 0.587 + bgCol.blue() * 0.114) < 128;
    QString menuHoverBg = isDarkBg ? "rgba(255, 255, 255, 0.12)" : "rgba(0, 0, 0, 0.08)";
    QString menuPressedBg = isDarkBg ? "rgba(255, 255, 255, 0.2)" : "rgba(0, 0, 0, 0.15)";

    QString dynamicRules = QString(
        R"(
        TimerWindow {
            background-color: %1;
        }
        QPushButton {
            color: %2;
            border: 1px solid %2;
        }
        QPushButton:hover {
            background-color: rgba(26, 115, 232, 0.08);
        }
        QPushButton:disabled {
            border: 1px solid #dadce0;
            color: #dadce0;
            background-color: %1;
        }
        QPushButton#menuBtn {
            background-color: transparent;
            color: %2;
            border: none;
        }
        QPushButton#menuBtn:hover {
            background-color: %3;
            color: %2;
        }
        QPushButton#menuBtn:pressed {
            background-color: %4;
            color: %2;
        }
        )"
    ).arg(currentBgColor).arg(currentTextColor).arg(menuHoverBg).arg(menuPressedBg);

    setStyleSheet(baseStyles + "\n" + dynamicRules);
}

void TimerWindow::applyTextOpacity() {
    QString labelStyle;
    if (isOpacityLocked) {
        labelStyle = QString("color: %1; font-size: 102px; font-weight: 500; background: transparent;").arg(currentTextColor);
    } else {
        qreal opacity = static_cast<qreal>(currentOpacityPercent) / 100.0;
        QColor col(currentTextColor);
        QString rgbaColor = QString("rgba(%1, %2, %3, %4)").arg(col.red()).arg(col.green()).arg(col.blue()).arg(opacity);
        labelStyle = QString("color: %1; font-size: 102px; font-weight: 500; background: transparent;").arg(rgbaColor);
    }

    minutesLabel->setStyleSheet(labelStyle);
    colonLabel->setStyleSheet(labelStyle);
    secondsLabel->setStyleSheet(labelStyle);
}

void TimerWindow::updateGui() {
    auto remaining = timer.remaining_seconds();

    if (remaining <= 0 && !timer.is_running()) {
        uiTimer->stop();
        minutesLabel->setText("00");
        colonLabel->setText(":");
        secondsLabel->setText("00");
        switchToSetupMode();
        currentSeconds = 0;
        updateGuiDisplay();
        return;
    }

    int minutes = remaining / 60;
    int secs = remaining % 60;

    minutesLabel->setText(QString("%1").arg(minutes, 2, 10, QChar('0')));
    colonLabel->setText(":");
    secondsLabel->setText(QString("%1").arg(secs, 2, 10, QChar('0')));
}

void TimerWindow::updateGuiDisplay() {
    int minutes = currentSeconds / 60;
    int secs = currentSeconds % 60;

    minutesLabel->setText(QString("%1").arg(minutes, 2, 10, QChar('0')));
    colonLabel->setText(":");
    secondsLabel->setText(QString("%1").arg(secs, 2, 10, QChar('0')));

    startBtn->setEnabled(currentSeconds > 0);
}

void TimerWindow::switchToSetupMode() {
    stackedLayout->setCurrentIndex(0);
}