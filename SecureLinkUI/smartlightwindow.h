#ifndef SMARTLIGHTWINDOW_H
#define SMARTLIGHTWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QMovie>
#include "toggleswitch.h"

class SmartLightWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SmartLightWindow(QWidget *parent = nullptr);

private slots:
    void toggleLight(bool checked);

private:
    QLabel *bulbLabel;
    QMovie *bulbMovie;
    QLabel *statusLabel;
    QComboBox *roomSelector;
    ToggleSwitch *switchControl;
    bool lightOn;
};

#endif // SMARTLIGHTWINDOW_H
