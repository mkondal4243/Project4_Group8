#ifndef DASHBOARDWINDOW_H
#define DASHBOARDWINDOW_H

#include <QMainWindow>

class DashboardWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DashboardWindow(QWidget *parent = nullptr);
    ~DashboardWindow();

signals:
    void openSmartLockPage();
    void openCameraPage();
    void openGarageDoorPage();
    void openSmartLightPage();
};

#endif // DASHBOARDWINDOW_H
