#include "dashboardwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QMouseEvent>

class ClickableTile : public QFrame {
    Q_OBJECT
public:
    explicit ClickableTile(const QString &icon, const QString &title, const QString &desc, QWidget *parent = nullptr)
        : QFrame(parent)
    {
        setStyleSheet(R"(
            QFrame {
                background-color: #222;
                border-radius: 16px;
                padding: 24px;
            }
            QFrame:hover {
                background-color: #2e2e2e;
            }
        )");

        QVBoxLayout *layout = new QVBoxLayout(this);

        QLabel *iconLabel = new QLabel(icon);
        iconLabel->setStyleSheet("font-size: 28px; color: gold;");
        iconLabel->setAlignment(Qt::AlignCenter);

        QLabel *titleLabel = new QLabel(title);
        titleLabel->setStyleSheet("font-size: 16px; color: white; font-weight: bold;");
        titleLabel->setAlignment(Qt::AlignCenter);

        QLabel *descLabel = new QLabel(desc);
        descLabel->setStyleSheet("font-size: 12px; color: #bbb;");
        descLabel->setAlignment(Qt::AlignCenter);

        layout->addWidget(iconLabel);
        layout->addSpacing(5);
        layout->addWidget(titleLabel);
        layout->addWidget(descLabel);
    }

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override {
        emit clicked();
        QFrame::mousePressEvent(event);
    }
};

#include "dashboardwindow.moc"

DashboardWindow::DashboardWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    QLabel *title = new QLabel("🔐 SecureLink Control Panel");
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: gold;");
    title->setAlignment(Qt::AlignCenter);

    QLabel *subtitle = new QLabel("Your smart defense system — real-time camera, locks, and automation.");
    subtitle->setStyleSheet("font-size: 14px; color: #bbb;");
    subtitle->setAlignment(Qt::AlignCenter);

    QHBoxLayout *tileLayout = new QHBoxLayout();

    auto *lockTile = new ClickableTile("🔒", "Front Door Lock", "Status: Locked");
    auto *cameraTile = new ClickableTile("📷", "Camera Feed", "Live View");
    auto *garageTile = new ClickableTile("🚪", "Garage Door", "Last opened: N/A");
    auto *lightTile = new ClickableTile("💡", "Smart Light", "Tap to toggle");

    connect(lockTile, &ClickableTile::clicked, this, &DashboardWindow::openSmartLockPage);
    connect(cameraTile, &ClickableTile::clicked, this, &DashboardWindow::openCameraPage);
    connect(garageTile, &ClickableTile::clicked, this, &DashboardWindow::openGarageDoorPage);
    connect(lightTile, &ClickableTile::clicked, this, &DashboardWindow::openSmartLightPage);

    tileLayout->addWidget(lockTile);
    tileLayout->addWidget(cameraTile);
    tileLayout->addWidget(garageTile);
    tileLayout->addWidget(lightTile);

    mainLayout->addSpacing(30);
    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(tileLayout);
    mainLayout->addStretch();

    setCentralWidget(central);
    setStyleSheet("background-color: #2b2b2b;");
}

DashboardWindow::~DashboardWindow() {}
