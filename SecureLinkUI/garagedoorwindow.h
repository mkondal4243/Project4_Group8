#ifndef GARAGEDOORWINDOW_H
#define GARAGEDOORWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QFrame>

class GarageDoorWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GarageDoorWindow(QWidget *parent = nullptr);

private slots:
    void toggleDoor();

private:
    QFrame *garageContainer;
    QFrame *garageDoor;
    QWidget *carWidget;
    QLabel *statusLabel;
    QPushButton *doorButton;
    bool isOpen;
    int doorHeight;
    QTimer *animationTimer;
};

#endif // GARAGEDOORWINDOW_H
