#ifndef ACCESSLOGSWIDGET_H
#define ACCESSLOGSWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>

class AccessLogsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AccessLogsWidget(QWidget *parent = nullptr);

private slots:
    void loadAccessLogs();
    void filterLogs();
    void exportLogsToFile();

private:
    void showFallbackLogs(const QString& reason); // <-- add this

    QTableWidget *logTable;
    QPushButton *refreshButton;
    QPushButton *exportButton;
    QLineEdit *usernameFilter;
    QLabel *statusLabel;
};

#endif // ACCESSLOGSWIDGET_H
