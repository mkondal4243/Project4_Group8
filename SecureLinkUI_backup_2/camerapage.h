#ifndef CAMERAPAGE_H
#define CAMERAPAGE_H

#include <QWidget>
#include <QLabel>
#include <QMap>
#include <QPushButton>

class CameraPage : public QWidget
{
    Q_OBJECT

public:
    explicit CameraPage(QWidget *parent = nullptr);

private slots:
    void switchToCamera(const QString &room);
    void zoomIn();
    void zoomOut();
    void takeScreenshot();

private:
    QLabel *noteLabel;
    QLabel *cameraFeedLabel;
    QLabel *cameraStatusLabel;
    QMap<QString, QString> roomFeeds;
    QPushButton *zoomInBtn;
    QPushButton *zoomOutBtn;
    QPushButton *screenshotBtn;
    int currentFontSize;
};

#endif // CAMERAPAGE_H
