#ifndef MOTIONALERTSWINDOW_H
#define MOTIONALERTSWINDOW_H

#include <QMainWindow>
#include <QListWidget>

class MotionAlertsWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MotionAlertsWindow(QWidget *parent = nullptr);

signals:
    void backToDashboardRequested();

private:
    QListWidget *alertList;
};

#endif // MOTIONALERTSWINDOW_H
