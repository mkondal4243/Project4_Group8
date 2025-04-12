#include "mainwindow.h"
#include "dashboardwindow.h"
#include "logtransferwindow.h"
#include "motionalertswidget.h"
#include "accesslogswidget.h"
#include "smartlightwindow.h"
#include "smartlockwindow.h"
#include "garagedoorwindow.h"
#include "camerapage.h"

#include <QListWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include "client_utils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);

    sidebar = new QListWidget(this);
    sidebar->setFixedWidth(220);
    sidebar->setStyleSheet(R"(
        QListWidget {
            background-color: #111;
            border: none;
            color: #ddd;
            font-size: 16px;
            font-weight: bold;
            padding: 16px;
        }
        QListWidget::item {
            padding: 18px 10px;
            margin-bottom: 6px;
        }
        QListWidget::item:hover {
            background-color: #222;
            color: #fff;
            border-radius: 10px;
        }
        QListWidget::item:selected {
            background-color: #b8860b;
            color: black;
            border-radius: 10px;
        }
    )");

    sidebar->addItem("📊 Dashboard");
    sidebar->addItem("📁 Log Transfer");
    sidebar->addItem("🚨 Motion Alerts");
    sidebar->addItem("📄 Access Logs");
    sidebar->addItem("💡 Smart Light");
    sidebar->addItem("📷 Camera Feed");
    sidebar->addItem("🚪 Garage Door");
    sidebar->addItem("🔒 Front Door Lock");

    stackedWidget = new QStackedWidget(this);

    dashboardWindow = new DashboardWindow();
    logTransferWindow = new LogTransferWindow();
    motionAlertsWidget = new MotionAlertsWidget();
    accessLogsWidget = new AccessLogsWidget();
    smartLightWindow = new SmartLightWindow();
    cameraPage = new CameraPage();
    garageDoorWindow = new GarageDoorWindow();
    smartLockWindow = new SmartLockWindow();

    stackedWidget->addWidget(dashboardWindow);
    stackedWidget->addWidget(logTransferWindow);
    stackedWidget->addWidget(motionAlertsWidget);
    stackedWidget->addWidget(accessLogsWidget);
    stackedWidget->addWidget(smartLightWindow);
    stackedWidget->addWidget(cameraPage);
    stackedWidget->addWidget(garageDoorWindow);
    stackedWidget->addWidget(smartLockWindow);

    connect(sidebar, &QListWidget::currentRowChanged, stackedWidget, &QStackedWidget::setCurrentIndex);

    connect(dashboardWindow, &DashboardWindow::openSmartLockPage, [=]() {
        stackedWidget->setCurrentIndex(7);
        sidebar->setCurrentRow(7);
    });
    connect(dashboardWindow, &DashboardWindow::openCameraPage, [=]() {
        stackedWidget->setCurrentIndex(5);
        sidebar->setCurrentRow(5);
    });
    connect(dashboardWindow, &DashboardWindow::openGarageDoorPage, [=]() {
        stackedWidget->setCurrentIndex(6);
        sidebar->setCurrentRow(6);
    });
    connect(dashboardWindow, &DashboardWindow::openSmartLightPage, [=]() {
        stackedWidget->setCurrentIndex(4);
        sidebar->setCurrentRow(4);
    });

    sidebar->setCurrentRow(0);

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(stackedWidget);
    setCentralWidget(central);

    setWindowTitle("SecureLink - Control Center");
    setStyleSheet("background-color: #2b2b2b;");

    // ✅ Server reconnect status logic
    QTimer *statusTimer = new QTimer(this);
    connect(statusTimer, &QTimer::timeout, this, [=]() {
        int sock = ClientUtils::createSocket();
        bool ok = ClientUtils::connectToServer(sock, "127.0.0.1", 9090);
        ClientUtils::closeSocket(sock);
        if (ok) {
            dashboardWindow->updateServerStatus("🟢 Server Status: Connected", "lightgreen");
        } else {
            dashboardWindow->updateServerStatus("🔴 Server Status: Disconnected", "red");
        }
    });
    statusTimer->start(5000);  // Check every 5 seconds
}

MainWindow::~MainWindow() {}
