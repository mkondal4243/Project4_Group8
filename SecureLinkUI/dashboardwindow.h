#ifndef DASHBOARDWINDOW_H
#define DASHBOARDWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

class DashboardWindow : public QMainWindow
{
    Q_OBJECT

public:
    DashboardWindow(QWidget *parent = nullptr);
    ~DashboardWindow();

public slots:
    void showDashboard();
    void showLogTransfer();
    void showMotionAlerts();
    void showAccessLogs();

private:
    QStackedWidget *stackedWidget;
};

#endif // DASHBOARDWINDOW_H
