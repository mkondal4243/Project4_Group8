#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QStackedWidget>

class DashboardWindow;
class LogTransferWindow;
class MotionAlertsWidget;
class AccessLogsWidget;
class SmartLightWindow;
class GarageDoorWindow;
class SmartLockWindow;
class CameraPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QListWidget *sidebar;
    QStackedWidget *stackedWidget;

    DashboardWindow *dashboardWindow;
    LogTransferWindow *logTransferWindow;
    MotionAlertsWidget *motionAlertsWidget;
    AccessLogsWidget *accessLogsWidget;
    SmartLightWindow *smartLightWindow;
    CameraPage *cameraPage;
    GarageDoorWindow *garageDoorWindow;
    SmartLockWindow *smartLockWindow;
};

#endif // MAINWINDOW_H
