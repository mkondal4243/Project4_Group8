#ifndef ACCESSLOGSWINDOW_H
#define ACCESSLOGSWINDOW_H

#include <QMainWindow>
#include <QListWidget>

class AccessLogsWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit AccessLogsWindow(QWidget *parent = nullptr);

signals:
    void backToDashboardRequested();

private:
    QListWidget *accessList;
};

#endif // ACCESSLOGSWINDOW_H
