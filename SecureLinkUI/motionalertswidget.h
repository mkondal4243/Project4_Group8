#ifndef MOTIONALERTSWIDGET_H
#define MOTIONALERTSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QPushButton>
#include <QSlider>

class MotionAlertsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MotionAlertsWidget(QWidget *parent = nullptr);

private slots:
    void simulateMotionEvent();
    void exportAlertsToFile();
    void updateMotionLabel(int value);
    void updateNoiseLabel(int value);

private:
    QLabel *motionStatus;
    QLabel *motionLabel;
    QLabel *noiseLabel;
    QLabel *lastExportLabel;

    QTimer *motionTimer;
    QSlider *motionSlider;
    QSlider *noiseSlider;
    QPushButton *exportButton;
    QStringList alertHistory;
};

#endif // MOTIONALERTSWIDGET_H
