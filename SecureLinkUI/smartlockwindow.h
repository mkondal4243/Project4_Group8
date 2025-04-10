#ifndef SMARTLOCKWINDOW_H
#define SMARTLOCKWINDOW_H

#include <QWidget>
#include <QLabel>
#include "toggleswitch.h"

class SmartLockWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SmartLockWindow(QWidget *parent = nullptr);

private slots:
    void toggleLock(bool checked);

private:
    QLabel *doorBlock;
    QLabel *knob;
    QLabel *speechLabel;
    QLabel *statusLabel;
    QWidget *doorContainer;
    ToggleSwitch *lockSwitch;
    bool isLocked;
};

#endif // SMARTLOCKWINDOW_H
