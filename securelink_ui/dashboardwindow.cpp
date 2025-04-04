#include "dashboardwindow.h"
#include "accesslogswindow.h"
#include "logtransferwindow.h"
#include "motionalertswindow.h"
#include "smartlightwindow.h"
#include "smartgaragewindow.h"
#include "camerapreviewwindow.h"

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QPalette>
#include <QLinearGradient>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDateTime>
#include <QTimer>
#include <QHBoxLayout>
#include <QFrame>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>

DashboardWindow::DashboardWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setFixedSize(720, 700);
    this->setWindowTitle("SecureLink – Dashboard");

    QPalette palette;
    QLinearGradient gradient(0, 0, 0, this->height());
    gradient.setColorAt(0.0, QColor("#e9f2f9"));
    gradient.setColorAt(1.0, QColor("#f9fcff"));
    palette.setBrush(QPalette::Window, gradient);
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    QWidget *container = new QWidget(this);
    container->setFixedSize(640, 640);
    container->setStyleSheet("background-color: white; border-radius: 16px;");
    container->move((this->width() - container->width()) / 2, (this->height() - container->height()) / 2);

    QVBoxLayout *mainLayout = new QVBoxLayout(container);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    QLabel *titleLabel = new QLabel("Dashboard – SecureLink Control Center", container);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #003147;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("color: #ccc;");
    mainLayout->addWidget(line);

    QLabel *aiStatus = new QLabel("🔒 Surveillance Uplink: Stable");
    aiStatus->setAlignment(Qt::AlignCenter);
    aiStatus->setStyleSheet("color: #0d3b66; font-size: 14px; font-weight: 600;");
    mainLayout->addWidget(aiStatus);

    QGraphicsOpacityEffect *aiEffect = new QGraphicsOpacityEffect(aiStatus);
    aiStatus->setGraphicsEffect(aiEffect);

    QPropertyAnimation *aiAnim = new QPropertyAnimation(aiEffect, "opacity");
    aiAnim->setDuration(1500);
    aiAnim->setStartValue(1.0);
    aiAnim->setEndValue(0.4);
    aiAnim->setEasingCurve(QEasingCurve::InOutQuad);
    aiAnim->setLoopCount(-1);
    aiAnim->start();

    QHBoxLayout *infoLayout = new QHBoxLayout();
    clockLabel = new QLabel(QDateTime::currentDateTime().toString("hh:mm:ss AP"));
    clockLabel->setFont(QFont("Arial", 12));
    clockLabel->setStyleSheet("color: #555;");
    infoLayout->addStretch();
    infoLayout->addWidget(clockLabel);
    infoLayout->addStretch();
    mainLayout->addLayout(infoLayout);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        clockLabel->setText(QDateTime::currentDateTime().toString("hh:mm:ss AP"));
    });
    timer->start(1000);

    QLabel *systemLabel = new QLabel("System Features");
    systemLabel->setStyleSheet("font-weight: bold; color: #222;");
    mainLayout->addWidget(systemLabel);

    QGridLayout *systemGrid = new QGridLayout();
    QStringList systemButtons = { "Log Transfer", "Access Logs", "Motion Alerts" };
    for (int i = 0; i < systemButtons.size(); ++i) {
        QPushButton *btn = new QPushButton(systemButtons[i]);
        btn->setFixedSize(180, 50);
        btn->setStyleSheet("QPushButton { background-color: #f2f4f8; color: #003147; border-radius: 8px; font-weight: 500; } QPushButton:hover { background-color: #e0e6ed; }");
        systemGrid->addWidget(btn, 0, i);

        if (systemButtons[i] == "Log Transfer") {
            connect(btn, &QPushButton::clicked, this, [=]() {
                LogTransferWindow *logWindow = new LogTransferWindow();
                connect(logWindow, &LogTransferWindow::backToDashboardRequested, this, [=]() {
                    this->show();
                });
                this->hide();
                logWindow->show();
            });
        } else if (systemButtons[i] == "Access Logs") {
            connect(btn, &QPushButton::clicked, this, [=]() {
                AccessLogsWindow *accessWindow = new AccessLogsWindow();
                connect(accessWindow, &AccessLogsWindow::backToDashboardRequested, this, [=]() {
                    this->show();
                });
                this->hide();
                accessWindow->show();
            });
        } else if (systemButtons[i] == "Motion Alerts") {
            connect(btn, &QPushButton::clicked, this, [=]() {
                MotionAlertsWindow *motionWindow = new MotionAlertsWindow();
                connect(motionWindow, &MotionAlertsWindow::backToDashboardRequested, this, [=]() {
                    this->show();
                });
                this->hide();
                motionWindow->show();
            });
        }
    }
    mainLayout->addLayout(systemGrid);

    QLabel *deviceLabel = new QLabel("Smart Devices");
    deviceLabel->setStyleSheet("font-weight: bold; color: #222;");
    mainLayout->addWidget(deviceLabel);

    QGridLayout *deviceGrid = new QGridLayout();
    QStringList deviceButtons = { "Smart Light", "Smart Garage", "Camera Preview" };
    for (int i = 0; i < deviceButtons.size(); ++i) {
        QPushButton *btn = new QPushButton(deviceButtons[i]);
        btn->setFixedSize(180, 50);
        btn->setStyleSheet("QPushButton { background-color: #f2f4f8; color: #003147; border-radius: 8px; font-weight: 500; } QPushButton:hover { background-color: #e0e6ed; }");
        deviceGrid->addWidget(btn, 0, i);

        if (deviceButtons[i] == "Smart Light") {
            connect(btn, &QPushButton::clicked, this, [=]() {
                SmartLightWindow *lightWindow = new SmartLightWindow();
                connect(lightWindow, &SmartLightWindow::backToDashboardRequested, this, [=]() {
                    this->show();
                });
                this->hide();
                lightWindow->show();
            });
        } else if (deviceButtons[i] == "Smart Garage") {
            connect(btn, &QPushButton::clicked, this, [=]() {
                SmartGarageWindow *garageWindow = new SmartGarageWindow();
                connect(garageWindow, &SmartGarageWindow::backToDashboardRequested, this, [=]() {
                    this->show();
                });
                this->hide();
                garageWindow->show();
            });
        } else if (deviceButtons[i] == "Camera Preview") {
            connect(btn, &QPushButton::clicked, this, [=]() {
                CameraPreviewWindow *cameraWindow = new CameraPreviewWindow();
                connect(cameraWindow, &CameraPreviewWindow::backToDashboardRequested, this, [=]() {
                    this->show();
                });
                this->hide();
                cameraWindow->show();
            });
        }
    }
    mainLayout->addLayout(deviceGrid);

    QPieSeries *series = new QPieSeries();
    series->append("Living Room", 40);
    series->append("Garage", 30);
    series->append("Kitchen", 20);
    series->append("Others", 10);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Room Usage Distribution");
    chart->setTitleFont(QFont("Arial", 12));
    chart->setBackgroundVisible(false);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setFixedHeight(200);
    mainLayout->addWidget(chartView, 0, Qt::AlignCenter);

    QPushButton *logoutButton = new QPushButton("Logout");
    logoutButton->setStyleSheet("QPushButton { background-color: #e53935; color: white; font-weight: bold; border-radius: 8px; padding: 10px; } QPushButton:hover { background-color: #c62828; }");
    mainLayout->addSpacing(10);
    mainLayout->addWidget(logoutButton, 0, Qt::AlignCenter);

    connect(logoutButton, &QPushButton::clicked, this, [=]() {
        emit logoutRequested();
        this->close();
    });
}
