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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);

    // Sidebar
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

    // Stack Area
    stackedWidget = new QStackedWidget(this);

    dashboardWindow = new DashboardWindow();
    logTransferWindow = new LogTransferWindow();
    motionAlertsWidget = new MotionAlertsWidget();
    accessLogsWidget = new AccessLogsWidget();
    smartLightWindow = new SmartLightWindow();
    cameraPage = new CameraPage();
    garageDoorWindow = new GarageDoorWindow();
    smartLockWindow = new SmartLockWindow();

    stackedWidget->addWidget(dashboardWindow);     // 0
    stackedWidget->addWidget(logTransferWindow);   // 1
    stackedWidget->addWidget(motionAlertsWidget);  // 2
    stackedWidget->addWidget(accessLogsWidget);    // 3
    stackedWidget->addWidget(smartLightWindow);    // 4
    stackedWidget->addWidget(cameraPage);          // 5
    stackedWidget->addWidget(garageDoorWindow);    // 6
    stackedWidget->addWidget(smartLockWindow);     // 7

    connect(sidebar, &QListWidget::currentRowChanged, stackedWidget, &QStackedWidget::setCurrentIndex);

    // Connect Dashboard tile signals
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
}

MainWindow::~MainWindow() {}
