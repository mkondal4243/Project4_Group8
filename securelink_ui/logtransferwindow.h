#ifndef LOGTRANSFERWINDOW_H
#define LOGTRANSFERWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QDateTime>


class LogTransferWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit LogTransferWindow(QWidget *parent = nullptr);

signals:
    void backToDashboardRequested();

private:
    QListWidget *logList;
};

#endif // LOGTRANSFERWINDOW_H
