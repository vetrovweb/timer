#include "SettingsWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QCheckBox>
#include <QFrame>
#include <QStyle>

SettingsWindow::SettingsWindow(int currentOpacity, bool lockOpacity,
                               const QString &currentTextColor, const QString &currentBgColor,
                               QWidget *parent)
    : QDialog(parent), selectedTextColor(currentTextColor), selectedBgColor(currentBgColor) {

    setWindowTitle("Налаштування");
    resize(360, 520);
    setObjectName("SettingsDialog");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(14);

    // Шапка
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(10);

    QLabel *gearIcon = new QLabel("⚙", this);
    gearIcon->setObjectName("SettingsHeaderIcon");

    QLabel *titleLabel = new QLabel("Налаштування", this);
    titleLabel->setObjectName("SettingsTitleLabel");

    headerLayout->addWidget(gearIcon);
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    QPushButton *closeBtn = new QPushButton("✕", this);
    closeBtn->setObjectName("SettingsCloseBtn");
    connect(closeBtn, &QPushButton::clicked, this, &SettingsWindow::close);
    headerLayout->addWidget(closeBtn);

    mainLayout->addLayout(headerLayout);
    mainLayout->addSpacing(5);

    // Палітри кольорів за макетом
    textColors = {"#17325c", "#202124", "#7f8c8d", "#e74c3c", "#e67e22", "#f1c40f", "#2ecc71", "#9b59b6", "#ffffff"};
    bgColors   = {"#ffffff", "#bdc3c7", "#7f8c8d", "#2c3e50", "#d6eaf8", "#ebdef0", "#fadbd8", "#d4efdf", "#fcf3cf"};

    // --- КАРТКА 1: Колір тексту ---
    QFrame *card1 = new QFrame(this);
    card1->setObjectName("SettingsCard");
    QVBoxLayout *card1Layout = new QVBoxLayout(card1);
    card1Layout->setContentsMargins(15, 15, 15, 15);
    card1Layout->setSpacing(8);

    QLabel *lblColorText = new QLabel("Колір тексту", this);
    lblColorText->setObjectName("CardTitle");
    card1Layout->addWidget(lblColorText);

    QHBoxLayout *palette1 = new QHBoxLayout();
    palette1->setSpacing(8);
    for (int i = 0; i < textColors.size(); ++i) {
        QPushButton *btn = new QPushButton(this);
        btn->setObjectName("settings_ColorCircle");
        btn->setProperty("colorHex", textColors[i]);

        bool isSelected = (textColors[i].toLower() == selectedTextColor.toLower());
        QString borderStyle = isSelected ? "border: 3px solid #1a73e8;" : "border: 1px solid rgba(0, 0, 0, 0.15);";

        btn->setStyleSheet(QString("background-color: %1; %2").arg(textColors[i], borderStyle));

        connect(btn, &QPushButton::clicked, this, &SettingsWindow::onTextColorClicked);
        textPaletteButtons.append(btn);
        palette1->addWidget(btn);
    }
    palette1->addStretch();
    card1Layout->addLayout(palette1);

    QLabel *sub1 = new QLabel("Застосовується до таймера, кнопок і тексту.", this);
    sub1->setObjectName("CardSubText");
    card1Layout->addWidget(sub1);
    mainLayout->addWidget(card1);

    // --- КАРТКА 2: Колір фону ---
    QFrame *card2 = new QFrame(this);
    card2->setObjectName("SettingsCard");
    QVBoxLayout *card2Layout = new QVBoxLayout(card2);
    card2Layout->setContentsMargins(15, 15, 15, 15);
    card2Layout->setSpacing(8);

    QLabel *lblColorBg = new QLabel("Колір фону", this);
    lblColorBg->setObjectName("CardTitle");
    card2Layout->addWidget(lblColorBg);

    QHBoxLayout *palette2 = new QHBoxLayout();
    palette2->setSpacing(8);
    for (int i = 0; i < bgColors.size(); ++i) {
        QPushButton *btn = new QPushButton(this);
        btn->setObjectName("settings_ColorCircle");
        btn->setProperty("colorHex", bgColors[i]);

        bool isSelected = (bgColors[i].toLower() == selectedBgColor.toLower());
        QString borderStyle = isSelected ? "border: 3px solid #1a73e8;" : "border: 1px solid rgba(0, 0, 0, 0.15);";

        btn->setStyleSheet(QString("background-color: %1; %2").arg(bgColors[i], borderStyle));

        connect(btn, &QPushButton::clicked, this, &SettingsWindow::onBgColorClicked);
        bgPaletteButtons.append(btn);
        palette2->addWidget(btn);
    }
    palette2->addStretch();
    card2Layout->addLayout(palette2);

    QLabel *sub2 = new QLabel("Фон таймера (вікна).", this);
    sub2->setObjectName("CardSubText");
    card2Layout->addWidget(sub2);
    mainLayout->addWidget(card2);

    // --- КАРТКА 3: Прозорість вікна та Чекбокс ---
    QFrame *card3 = new QFrame(this);
    card3->setObjectName("SettingsCard");
    QVBoxLayout *card3Layout = new QVBoxLayout(card3);
    card3Layout->setContentsMargins(15, 15, 15, 15);
    card3Layout->setSpacing(10);

    QLabel *lblOpacity = new QLabel("Прозорість вікна", this);
    lblOpacity->setObjectName("CardTitle");
    card3Layout->addWidget(lblOpacity);

    QHBoxLayout *sliderLayout = new QHBoxLayout();
    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(40, 100);
    slider->setValue(currentOpacity);
    slider->setObjectName("SettingsSlider");

    percentLbl = new QLabel(QString("%1%").arg(currentOpacity), this);
    percentLbl->setObjectName("PercentLabel");

    sliderLayout->addWidget(slider);
    sliderLayout->addWidget(percentLbl);
    card3Layout->addLayout(sliderLayout);

    QLabel *sub3 = new QLabel("Загальна прозорість вікна таймера.", this);
    sub3->setObjectName("CardSubText");
    card3Layout->addWidget(sub3);

    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setObjectName("SeparatorLine");
    card3Layout->addWidget(line);

    checkBox = new QCheckBox("Заблокувати прозорість таймера", this);
    checkBox->setChecked(lockOpacity);
    checkBox->setObjectName("SettingsCheckBox");
    card3Layout->addWidget(checkBox);

    QLabel *sub4 = new QLabel("Цифри таймера залишаються повністю непрозорими\nі не стають напівпрозорими разом із вікном.", this);
    sub4->setObjectName("CardSubText");
    card3Layout->addWidget(sub4);

    mainLayout->addWidget(card3);
    mainLayout->addStretch();

    // Сигнали
    connect(slider, &QSlider::valueChanged, this, &SettingsWindow::onSliderValueChanged);
    connect(checkBox, &QCheckBox::toggled, this, &SettingsWindow::lockOpacityToggled);
}

void SettingsWindow::onSliderValueChanged(int value) {
    percentLbl->setText(QString("%1%").arg(value));
    emit opacityChanged(value);
}

void SettingsWindow::onTextColorClicked() {
    QPushButton *clickedBtn = qobject_cast<QPushButton*>(sender());
    if (!clickedBtn) return;

    QString colorHex = clickedBtn->property("colorHex").toString();
    selectedTextColor = colorHex;

    for (auto btn : textPaletteButtons) {
        QString hex = btn->property("colorHex").toString();
        bool match = (hex.toLower() == colorHex.toLower());
        QString borderStyle = match ? "border: 3px solid #1a73e8;" : "border: 1px solid rgba(0, 0, 0, 0.15);";
        btn->setStyleSheet(QString("background-color: %1; %2").arg(hex, borderStyle));
    }

    emit textColorChanged(colorHex);
}

void SettingsWindow::onBgColorClicked() {
    QPushButton *clickedBtn = qobject_cast<QPushButton*>(sender());
    if (!clickedBtn) return;

    QString colorHex = clickedBtn->property("colorHex").toString();
    selectedBgColor = colorHex;

    for (auto btn : bgPaletteButtons) {
        QString hex = btn->property("colorHex").toString();
        bool match = (hex.toLower() == colorHex.toLower());
        QString borderStyle = match ? "border: 3px solid #1a73e8;" : "border: 1px solid rgba(0, 0, 0, 0.15);";
        btn->setStyleSheet(QString("background-color: %1; %2").arg(hex, borderStyle));
    }

    emit bgColorChanged(colorHex);
}