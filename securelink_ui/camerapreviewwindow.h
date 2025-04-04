#ifndef CAMERAPREVIEWWINDOW_H
#define CAMERAPREVIEWWINDOW_H

#include <QMainWindow>

class CameraPreviewWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CameraPreviewWindow(QWidget *parent = nullptr);

signals:
    void backToDashboardRequested();
};

#endif // CAMERAPREVIEWWINDOW_H
