#ifndef DASHBOARDWINDOW_H
#define DASHBOARDWINDOW_H

#include <QMainWindow>
#include <QLabel>

class DashboardWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DashboardWindow(QWidget *parent = nullptr);
    ~DashboardWindow();  // <- Important for the linker

public slots:
    void updateServerStatus(const QString& message, const QString& color);

private:
    QLabel *statusIndicator;

signals:
    void openSmartLockPage();
    void openCameraPage();
    void openGarageDoorPage();
    void openSmartLightPage();
};

#endif // DASHBOARDWINDOW_H
