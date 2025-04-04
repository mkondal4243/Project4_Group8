#include "smartgaragewindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QFrame>

SmartGarageWindow::SmartGarageWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setFixedSize(680, 460);
    this->setWindowTitle("SecureLink – Smart Garage");

    QWidget *central = new QWidget(this);
    central->setStyleSheet("background-color: #f7f9fa; border-radius: 12px;");
    central->setFixedSize(600, 400);
    central->move((width() - central->width()) / 2, (height() - central->height()) / 2);

    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(15);

    QLabel *title = new QLabel("Garage Control Panel");
    title->setStyleSheet("font-size: 18px; font-weight: bold; color: #1a237e;");
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: #ccc;");
    layout->addWidget(line);

    QLabel *status = new QLabel("🚗 Garage Door is currently CLOSED");
    status->setStyleSheet("font-size: 14px; color: #333;");
    status->setAlignment(Qt::AlignCenter);
    layout->addWidget(status);

    QPushButton *toggleBtn = new QPushButton("Toggle Garage Door");
    toggleBtn->setStyleSheet("QPushButton { background-color: #1e88e5; color: white; border-radius: 6px; height: 36px; } QPushButton:hover { background-color: #1565c0; }");
    layout->addWidget(toggleBtn);

    QPushButton *backBtn = new QPushButton("Back to Dashboard");
    backBtn->setStyleSheet("QPushButton { background-color: #78909c; color: white; border-radius: 6px; height: 32px; } QPushButton:hover { background-color: #607d8b; }");
    layout->addWidget(backBtn);

    connect(toggleBtn, &QPushButton::clicked, this, [=]() {
        static bool open = false;
        open = !open;
        status->setText(open ? "🚪 Garage Door is now OPEN" : "🚗 Garage Door is now CLOSED");
    });

    connect(backBtn, &QPushButton::clicked, this, [=]() {
        emit backToDashboardRequested();
        this->close();
    });
}
