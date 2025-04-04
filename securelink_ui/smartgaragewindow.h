#ifndef SMARTGARAGEWINDOW_H
#define SMARTGARAGEWINDOW_H

#include <QMainWindow>

class SmartGarageWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SmartGarageWindow(QWidget *parent = nullptr);

signals:
    void backToDashboardRequested();
};

#endif // SMARTGARAGEWINDOW_H
