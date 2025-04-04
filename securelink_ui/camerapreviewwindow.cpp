#include "camerapreviewwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QFrame>

CameraPreviewWindow::CameraPreviewWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setFixedSize(680, 460);
    this->setWindowTitle("SecureLink – Camera Preview");

    QWidget *central = new QWidget(this);
    central->setStyleSheet("background-color: #f3f4f6; border-radius: 12px;");
    central->setFixedSize(600, 400);
    central->move((width() - central->width()) / 2, (height() - central->height()) / 2);

    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(18);

    QLabel *title = new QLabel("Live Camera Feed");
    title->setStyleSheet("font-size: 18px; font-weight: bold; color: #263238;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: #ccc;");
    layout->addWidget(line);

    QLabel *cameraView = new QLabel("📷 [Simulated Camera Feed]");
    cameraView->setStyleSheet("font-size: 14px; color: #444; padding: 10px; background-color: #e0e0e0; border-radius: 8px;");
    cameraView->setAlignment(Qt::AlignCenter);
    layout->addWidget(cameraView);

    QPushButton *backBtn = new QPushButton("Back to Dashboard");
    backBtn->setStyleSheet("QPushButton { background-color: #90a4ae; color: white; border-radius: 6px; height: 32px; } QPushButton:hover { background-color: #607d8b; }");
    layout->addWidget(backBtn);

    connect(backBtn, &QPushButton::clicked, this, [=]() {
        emit backToDashboardRequested();
        this->close();
    });
}
