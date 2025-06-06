#include "camerapage.h"
#include "client_utils.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDateTime>
#include <QButtonGroup>
#include <QFileDialog>

CameraPage::CameraPage(QWidget *parent)
    : QWidget(parent), currentFontSize(18)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *title = new QLabel("Live Camera Feed", this);
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: gold;");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    noteLabel = new QLabel("Select a room to view live camera feed.", this);
    noteLabel->setStyleSheet("font-size: 13px; color: #aaa;");
    noteLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(noteLabel);

    QHBoxLayout *contentLayout = new QHBoxLayout();

    // 📺 Feed Preview Area
    QVBoxLayout *feedLayout = new QVBoxLayout();

    cameraFeedLabel = new QLabel("No camera selected", this);
    cameraFeedLabel->setStyleSheet(QString("font-size: %1px; color: #bbb; background-color: #111; border: 2px dashed #444; border-radius: 10px; padding: 40px;").arg(currentFontSize));
    cameraFeedLabel->setAlignment(Qt::AlignCenter);
    feedLayout->addWidget(cameraFeedLabel);

    // 📷 Camera Controls
    QHBoxLayout *buttonRow = new QHBoxLayout();
    zoomInBtn = new QPushButton("Zoom In", this);
    zoomOutBtn = new QPushButton("Zoom Out", this);
    screenshotBtn = new QPushButton("Take Screenshot", this);
    downloadSnapshotBtn = new QPushButton("📸 Download Snapshot", this);

    zoomInBtn->setStyleSheet("padding: 6px 14px; background-color: #333; color: white;");
    zoomOutBtn->setStyleSheet("padding: 6px 14px; background-color: #333; color: white;");
    screenshotBtn->setStyleSheet("padding: 6px 14px; background-color: #333; color: white;");
    downloadSnapshotBtn->setStyleSheet("padding: 6px 14px; background-color: darkgreen; color: white;");

    buttonRow->addWidget(zoomInBtn);
    buttonRow->addWidget(zoomOutBtn);
    buttonRow->addWidget(screenshotBtn);
    buttonRow->addWidget(downloadSnapshotBtn);
    feedLayout->addLayout(buttonRow);

    connect(zoomInBtn, &QPushButton::clicked, this, &CameraPage::zoomIn);
    connect(zoomOutBtn, &QPushButton::clicked, this, &CameraPage::zoomOut);
    connect(screenshotBtn, &QPushButton::clicked, this, &CameraPage::takeScreenshot);
    connect(downloadSnapshotBtn, &QPushButton::clicked, this, [=]() {
    QString filePath = QFileDialog::getSaveFileName(this, "Save Snapshot", "snapshot_from_server.jpg", "JPEG Files (*.jpg)");
    if (!filePath.isEmpty()) {
        std::string result = ClientUtils::saveSnapshotToFile();  // Still saves to temp path
        QFile::copy("snapshot_from_server.jpg", filePath);       // Copy to user selected path
        cameraStatusLabel->setText("📥 Snapshot saved at " + filePath + " at " + QDateTime::currentDateTime().toString("hh:mm:ss"));
    } else {
        cameraStatusLabel->setText("⚠️ Snapshot canceled.");
    }
});

    // 📢 Status label
    cameraStatusLabel = new QLabel("Choose a room from the right panel", this);
    cameraStatusLabel->setStyleSheet("font-size: 14px; color: #888;");
    cameraStatusLabel->setAlignment(Qt::AlignCenter);
    feedLayout->addWidget(cameraStatusLabel);

    contentLayout->addLayout(feedLayout, 2);

    // 🎛️ Room Switch
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    QLabel *switchTitle = new QLabel("Switch Camera View", this);
    switchTitle->setStyleSheet("font-size: 16px; color: white; font-weight: bold;");
    switchTitle->setAlignment(Qt::AlignCenter);
    buttonLayout->addWidget(switchTitle);

    roomFeeds = {
        {"Living Room", "Connected to Living Room."},
        {"Front Door", "Monitoring Front Door."},
        {"Garage", "Garage camera active."},
        {"Backyard", "Backyard feed ready."},
        {"Kitchen", "Kitchen camera streaming."},
        {"Hallway", "Hallway cam on duty."}
    };

    QButtonGroup *btnGroup = new QButtonGroup(this);
    for (auto it = roomFeeds.begin(); it != roomFeeds.end(); ++it) {
        QString roomName = it.key();
        QPushButton *btn = new QPushButton(roomName, this);
        btn->setStyleSheet("padding: 10px; background-color: #2e2e2e; color: white; font-weight: bold; border-radius: 8px;");
        connect(btn, &QPushButton::clicked, this, [=]() {
            switchToCamera(roomName);
        });
        buttonLayout->addWidget(btn);
        btnGroup->addButton(btn);
    }

    buttonLayout->addStretch();
    contentLayout->addLayout(buttonLayout, 1);

    mainLayout->addLayout(contentLayout);
    mainLayout->addStretch();

    setStyleSheet("background-color: #2b2b2b;");
}

void CameraPage::switchToCamera(const QString &room)
{
    if (!roomFeeds.contains(room)) {
        cameraFeedLabel->setText("❌ Invalid room selected.");
        return;
    }

    std::string response = ClientUtils::sendCameraRequest(room.toStdString());

    if (cameraFeedLabel) {
        QString message;

        try {
            message = QString::fromStdString(response);
        } catch (...) {
            message = "⚠️ Invalid camera response received.";
        }

        if (message.trimmed().isEmpty()) {
            message = "⚠️ Failed to load feed for " + room;
        }

        cameraFeedLabel->setText(message);
        QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
        cameraStatusLabel->setText("🕒 Last switched at " + timestamp);
    }
}

void CameraPage::zoomIn()
{
    if (currentFontSize < 36) {
        currentFontSize += 2;
        cameraFeedLabel->setStyleSheet(QString("font-size: %1px; color: #bbb; background-color: #111; border: 2px dashed #444; border-radius: 10px; padding: 40px;").arg(currentFontSize));
    }
}

void CameraPage::zoomOut()
{
    if (currentFontSize > 12) {
        currentFontSize -= 2;
        cameraFeedLabel->setStyleSheet(QString("font-size: %1px; color: #bbb; background-color: #111; border: 2px dashed #444; border-radius: 10px; padding: 40px;").arg(currentFontSize));
    }
}

void CameraPage::takeScreenshot()
{
    cameraStatusLabel->setText("📸 Screenshot simulated at " + QDateTime::currentDateTime().toString("hh:mm:ss"));
}
