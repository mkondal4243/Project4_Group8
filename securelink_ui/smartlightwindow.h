#ifndef SMARTLIGHTWINDOW_H
#define SMARTLIGHTWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>

class SmartLightWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit SmartLightWindow(QWidget *parent = nullptr);

signals:
    void backToDashboardRequested();

private:
    QLabel *statusLabel;
    QPushButton *toggleBtn;
    QListWidget *historyList;
    bool isLightOn = false;
};

#endif // SMARTLIGHTWINDOW_H
