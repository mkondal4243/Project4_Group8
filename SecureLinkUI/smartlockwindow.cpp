#include "smartlockwindow.h"
#include "client_utils.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QDebug>
#include <QTimer>

SmartLockWindow::SmartLockWindow(QWidget *parent)
    : QWidget(parent), isLocked(true)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *title = new QLabel("🔐 Front Door Lock", this);
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: gold;");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    QLabel *desc = new QLabel("Secure your front door with a tap.", this);
    desc->setStyleSheet("font-size: 13px; color: #aaa;");
    desc->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(desc);

    // 🚪 Door Container (with door + knob inside)
    doorContainer = new QWidget(this);
    QVBoxLayout *doorLayout = new QVBoxLayout(doorContainer);
    doorLayout->setContentsMargins(0, 0, 0, 0);
    doorLayout->setAlignment(Qt::AlignCenter);

    // 🚪 Door Block
    doorBlock = new QLabel(doorContainer);
    doorBlock->setFixedSize(130, 220);
    doorBlock->setStyleSheet("background-color: saddlebrown; border-radius: 6px; border: 2px solid #222;");
    doorBlock->setAlignment(Qt::AlignCenter);

    // 🔘 Knob (positioned manually but *inside* doorContainer now)
    knob = new QLabel(doorContainer);
    knob->setFixedSize(14, 14);
    knob->setStyleSheet("background-color: black; border-radius: 7px;");

    // Set knob position relative to door
    QTimer::singleShot(0, this, [=]() {
        int knobX = doorBlock->x() + doorBlock->width() - 22;
        int knobY = doorBlock->y() + (doorBlock->height() / 2) - 7;
        knob->move(knobX, knobY);
    });

    doorLayout->addWidget(doorBlock);
    doorContainer->setLayout(doorLayout);
    mainLayout->addWidget(doorContainer, 0, Qt::AlignCenter);

    // 🗨️ Speech Bubble
    speechLabel = new QLabel(this);
    speechLabel->setText("");
    speechLabel->setStyleSheet("background-color: #333; color: white; font-size: 14px; padding: 8px 16px; border-radius: 10px;");
    speechLabel->setAlignment(Qt::AlignCenter);
    speechLabel->setVisible(false);
    mainLayout->addWidget(speechLabel, 0, Qt::AlignCenter);

    // 🔁 Toggle Switch
    lockSwitch = new ToggleSwitch(this);
    lockSwitch->setFixedSize(60, 30);
    lockSwitch->setCursor(Qt::PointingHandCursor);
    connect(lockSwitch, &ToggleSwitch::toggled, this, &SmartLockWindow::toggleLock);
    mainLayout->addWidget(lockSwitch, 0, Qt::AlignCenter);

    // 🔒 Status Label
    statusLabel = new QLabel("Status: LOCKED", this);
    statusLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: lightgreen;");
    statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(statusLabel);

    setLayout(mainLayout);
    setStyleSheet("background-color: #2b2b2b;");
}

void SmartLockWindow::toggleLock(bool checked)
{
    isLocked = !isLocked;

    int sock = ClientUtils::createSocket();
    if (sock >= 0 && ClientUtils::connectToServer(sock, "127.0.0.1", 9090)) {
        ClientUtils::sendMessage(sock, "LOCK_TOGGLE");
        QString response = QString::fromStdString(ClientUtils::receiveMessage(sock));
        qDebug() << "🔐 Server Response:" << response;
        ClientUtils::closeSocket(sock);
    }

    if (isLocked) {
        doorBlock->setStyleSheet("background-color: saddlebrown; border-radius: 6px; border: 2px solid #222;");
        knob->setStyleSheet("background-color: black; border-radius: 7px;");
        speechLabel->setVisible(false);  // ✅ fixed
        statusLabel->setText("Status: LOCKED");
        statusLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: lightgreen;");
    } else {
        doorBlock->setStyleSheet("background-color: peru; border: 2px dashed red; border-radius: 6px;");
        knob->setStyleSheet("background-color: black; border-radius: 7px;");
        speechLabel->setText("Welcome, welcome! Don’t be shy – come inside! 🚪");  // ✅ fixed
        speechLabel->setVisible(true);  // ✅ fixed
        statusLabel->setText("Status: UNLOCKED");
        statusLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: red;");
    }
}

