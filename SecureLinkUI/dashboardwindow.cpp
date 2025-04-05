#include "dashboardwindow.h"
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QListWidget>
#include <QListWidgetItem>
#include <QStackedWidget>

DashboardWindow::DashboardWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);

    // Sidebar
    QListWidget *sideMenu = new QListWidget(this);
    sideMenu->setFixedWidth(180);
    sideMenu->setStyleSheet(R"(
        QListWidget {
            background-color: #1a1a1a;
            color: white;
            font-size: 14px;
            padding: 10px;
        }
        QListWidget::item {
            padding: 12px;
        }
        QListWidget::item:selected {
            background-color: #b8860b;
            color: black;
        }
    )");
    sideMenu->addItem("🔐 Dashboard");
    sideMenu->addItem("📁 Log Transfer");
    sideMenu->addItem("🚨 Motion Alerts");
    sideMenu->addItem("📄 Access Logs");

    // Page stack
    stackedWidget = new QStackedWidget(this);

    // Dashboard Page
    QWidget *dashPage = new QWidget();
    QVBoxLayout *dashLayout = new QVBoxLayout(dashPage);

    QLabel *welcome = new QLabel("🔐 SecureLink Control Panel");
    welcome->setStyleSheet("font-size: 22px; font-weight: bold; color: gold;");
    welcome->setAlignment(Qt::AlignCenter);

    QLabel *tagline = new QLabel("Your home’s smart defense system — view logs, monitor motion, and more.");
    tagline->setStyleSheet("font-size: 14px; color: #bbb;");
    tagline->setAlignment(Qt::AlignCenter);

    QHBoxLayout *tilesLayout = new QHBoxLayout();
    auto tile = [](QString icon, QString title, QString desc) {
        QFrame *frame = new QFrame();
        frame->setStyleSheet("QFrame { background-color: #222; border-radius: 12px; padding: 20px; }");
        QVBoxLayout *layout = new QVBoxLayout(frame);
        QLabel *iconLabel = new QLabel(icon); iconLabel->setStyleSheet("font-size: 24px; color: gold;");
        QLabel *titleLabel = new QLabel(title); titleLabel->setStyleSheet("font-size: 16px; color: white; font-weight: bold;");
        QLabel *descLabel = new QLabel(desc); descLabel->setStyleSheet("font-size: 12px; color: #bbb;");
        iconLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setAlignment(Qt::AlignCenter);
        descLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(iconLabel); layout->addSpacing(5);
        layout->addWidget(titleLabel); layout->addWidget(descLabel);
        return frame;
    };

    tilesLayout->addWidget(tile("🔒", "Smart Lock", "Status: Locked"));
    tilesLayout->addWidget(tile("📷", "Camera Feed", "Live Monitoring"));
    tilesLayout->addWidget(tile("🚪", "Garage Door", "Closed at 9:12 AM"));

    dashLayout->addSpacing(30);
    dashLayout->addWidget(welcome);
    dashLayout->addWidget(tagline);
    dashLayout->addSpacing(20);
    dashLayout->addLayout(tilesLayout);
    dashLayout->addStretch();

    // Log Transfer Page
    QWidget *logPage = new QLabel("📁 Log Transfer Page Coming Soon");

    // Motion Alerts Page
    QWidget *motionPage = new QLabel("🚨 Motion Alerts Page Coming Soon");

    // Access Logs Page
    QWidget *accessPage = new QWidget();
    QVBoxLayout *accessLayout = new QVBoxLayout(accessPage);

    QLabel *accessTitle = new QLabel("📄 Access Logs Summary");
    accessTitle->setStyleSheet("font-size: 20px; font-weight: bold; color: gold;");
    accessTitle->setAlignment(Qt::AlignCenter);

    QLabel *accessDesc = new QLabel("You’ll be able to view login attempts, timestamped entries, and suspicious access activity here.");
    accessDesc->setStyleSheet("font-size: 14px; color: #aaa;");
    accessDesc->setAlignment(Qt::AlignCenter);
    accessDesc->setWordWrap(true);

    QFrame *placeholderBox = new QFrame();
    placeholderBox->setStyleSheet("QFrame { border: 2px dashed #444; background-color: #1e1e1e; border-radius: 12px; margin: 40px; }");
    QVBoxLayout *placeholderLayout = new QVBoxLayout(placeholderBox);
    QLabel *noLogs = new QLabel("🔐 No logs to display yet.");
    noLogs->setStyleSheet("font-size: 16px; color: #888;");
    noLogs->setAlignment(Qt::AlignCenter);
    placeholderLayout->addStretch(); placeholderLayout->addWidget(noLogs); placeholderLayout->addStretch();

    accessLayout->addStretch();
    accessLayout->addWidget(accessTitle);
    accessLayout->addSpacing(10);
    accessLayout->addWidget(accessDesc);
    accessLayout->addWidget(placeholderBox);
    accessLayout->addStretch();

    // Add pages
    stackedWidget->addWidget(dashPage);      // 0
    stackedWidget->addWidget(logPage);       // 1
    stackedWidget->addWidget(motionPage);    // 2
    stackedWidget->addWidget(accessPage);    // 3

    // Final layout
    mainLayout->addWidget(sideMenu);
    mainLayout->addWidget(stackedWidget);

    connect(sideMenu, &QListWidget::currentRowChanged, stackedWidget, &QStackedWidget::setCurrentIndex);
}

DashboardWindow::~DashboardWindow() {}

void DashboardWindow::showDashboard() { qDebug() << "Dashboard clicked"; stackedWidget->setCurrentIndex(0); }
void DashboardWindow::showLogTransfer() { qDebug() << "Log Transfer clicked"; stackedWidget->setCurrentIndex(1); }
void DashboardWindow::showMotionAlerts() { qDebug() << "Motion Alerts clicked"; stackedWidget->setCurrentIndex(2); }
void DashboardWindow::showAccessLogs() { qDebug() << "Access Logs clicked"; stackedWidget->setCurrentIndex(3); }
