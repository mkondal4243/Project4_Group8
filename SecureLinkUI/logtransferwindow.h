#ifndef LOGTRANSFERWINDOW_H
#define LOGTRANSFERWINDOW_H

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>

class LogTransferWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LogTransferWindow(QWidget *parent = nullptr);

private slots:
    void receiveLogs();
    void saveLogsToFile();

private:
    QTextEdit *logDisplay;
    QPushButton *receiveButton;
    QPushButton *saveButton;
    QLabel *timestampLabel;
    QLabel *statusLabel;
};

#endif // LOGTRANSFERWINDOW_H
