#pragma once

#include <QDialog>
#include <QString>
#include <QList>

class QSlider;
class QCheckBox;
class QLabel;
class QPushButton;

class SettingsWindow : public QDialog {
    Q_OBJECT

public:
    explicit SettingsWindow(int currentOpacity, bool lockOpacity,
                            const QString &currentTextColor, const QString &currentBgColor,
                            QWidget *parent = nullptr);
    ~SettingsWindow() = default;

    signals:
        void opacityChanged(int opacityPercent);
    void lockOpacityToggled(bool locked);
    void textColorChanged(const QString &colorHex);
    void bgColorChanged(const QString &colorHex);

private slots:
    void onSliderValueChanged(int value);
    void onTextColorClicked();
    void onBgColorClicked();

private:
    QSlider *slider;
    QLabel *percentLbl;
    QCheckBox *checkBox;

    QString selectedTextColor;
    QString selectedBgColor;

    QList<QString> textColors;
    QList<QString> bgColors;

    QList<QPushButton*> textPaletteButtons;
    QList<QPushButton*> bgPaletteButtons;
};